#include "hnswlib/hnswalg.h"
#include "hnswlib/space_ip.h"
#include "hnswlib/space_l2.h"

#include <fstream>
#include <hnswlib/hnswlib.h>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

struct Item {
    int index;
    std::string key;
    std::vector<float> embedding;
};

void normalize(std::vector<float>& v) {
    float sum = 0;
    for (const auto& val : v) {
        sum += val * val;
    }
    float norm = std::sqrt(sum);
    for (auto& val : v) {
        val /= norm;
    }
}

template <typename dist_t>
class MyHNSW : public hnswlib::HierarchicalNSW<dist_t> {
  public:
    using hnswlib::HierarchicalNSW<dist_t>::HierarchicalNSW;

    void newSaveIndex(const std::string& location) {
        std::ofstream output(location, std::ios::binary);
        std::streampos position;

        hnswlib::writeBinaryPOD(output, "aaaaaaa");

        hnswlib::writeBinaryPOD(output, this->offsetLevel0_);
        hnswlib::writeBinaryPOD(output, this->max_elements_);
        hnswlib::writeBinaryPOD(output, this->cur_element_count);
        hnswlib::writeBinaryPOD(output, this->size_data_per_element_);
        hnswlib::writeBinaryPOD(output, this->label_offset_);
        hnswlib::writeBinaryPOD(output, this->offsetData_);
        hnswlib::writeBinaryPOD(output, this->maxlevel_);
        hnswlib::writeBinaryPOD(output, this->enterpoint_node_);
        hnswlib::writeBinaryPOD(output, this->maxM_);

        hnswlib::writeBinaryPOD(output, this->maxM0_);
        hnswlib::writeBinaryPOD(output, this->M_);
        hnswlib::writeBinaryPOD(output, this->mult_);
        hnswlib::writeBinaryPOD(output, this->ef_construction_);

        output.write(this->data_level0_memory_,
                     this->cur_element_count * this->size_data_per_element_);

        for (size_t i = 0; i < this->cur_element_count; i++) {
            unsigned int linkListSize =
                this->element_levels_[i] > 0
                    ? this->size_links_per_element_ * this->element_levels_[i]
                    : 0;
            hnswlib::writeBinaryPOD(output, linkListSize);
            if (linkListSize)
                output.write(this->linkLists_[i], linkListSize);
        }
        output.close();
    }

    void newLoadIndex(const std::string& location,
                      hnswlib::SpaceInterface<dist_t>* s,
                      size_t max_elements_i = 0) {
        std::ifstream input(location, std::ios::binary);

        if (!input.is_open())
            throw std::runtime_error("Cannot open file");

        // get file size:
        input.seekg(0, input.end);
        std::streampos total_filesize = input.tellg();
        input.seekg(0, input.beg);

        char buffer[8];
        hnswlib::readBinaryPOD(input, buffer);
        std::cout << buffer << std::endl;
        hnswlib::readBinaryPOD(input, this->offsetLevel0_);
        hnswlib::readBinaryPOD(input, this->max_elements_);
        hnswlib::readBinaryPOD(input, this->cur_element_count);

        size_t max_elements = max_elements_i;
        if (max_elements < this->cur_element_count)
            max_elements = this->max_elements_;
        this->max_elements_ = max_elements;
        hnswlib::readBinaryPOD(input, this->size_data_per_element_);
        hnswlib::readBinaryPOD(input, this->label_offset_);
        hnswlib::readBinaryPOD(input, this->offsetData_);
        hnswlib::readBinaryPOD(input, this->maxlevel_);
        hnswlib::readBinaryPOD(input, this->enterpoint_node_);

        hnswlib::readBinaryPOD(input, this->maxM_);
        hnswlib::readBinaryPOD(input, this->maxM0_);
        hnswlib::readBinaryPOD(input, this->M_);
        hnswlib::readBinaryPOD(input, this->mult_);
        hnswlib::readBinaryPOD(input, this->ef_construction_);

        this->data_size_ = s->get_data_size();
        this->fstdistfunc_ = s->get_dist_func();
        this->dist_func_param_ = s->get_dist_func_param();

        auto pos = input.tellg();

        /// Optional - check if index is ok:
        input.seekg(this->cur_element_count * this->size_data_per_element_,
                    input.cur);
        for (size_t i = 0; i < this->cur_element_count; i++) {
            if (input.tellg() < 0 || input.tellg() >= total_filesize) {
                throw std::runtime_error(
                    "Index seems to be corrupted or unsupported");
            }

            unsigned int linkListSize;
            hnswlib::readBinaryPOD(input, linkListSize);
            if (linkListSize != 0) {
                input.seekg(linkListSize, input.cur);
            }
        }

        // throw exception if it either corrupted or old index
        if (input.tellg() != total_filesize)
            throw std::runtime_error(
                "Index seems to be corrupted or unsupported");

        input.clear();
        /// Optional check end

        input.seekg(pos, input.beg);

        this->data_level0_memory_ =
            (char*)malloc(max_elements * this->size_data_per_element_);
        if (this->data_level0_memory_ == nullptr)
            throw std::runtime_error(
                "Not enough memory: loadIndex failed to allocate level0");
        input.read(this->data_level0_memory_,
                   this->cur_element_count * this->size_data_per_element_);

        this->size_links_per_element_ =
            this->maxM_ * sizeof(hnswlib::tableint) +
            sizeof(hnswlib::linklistsizeint);

        this->size_links_level0_ = this->maxM0_ * sizeof(hnswlib::tableint) +
                                   sizeof(hnswlib::linklistsizeint);
        std::vector<std::mutex>(max_elements).swap(this->link_list_locks_);
        std::vector<std::mutex>(this->MAX_LABEL_OPERATION_LOCKS)
            .swap(this->label_op_locks_);

        this->visited_list_pool_ =
            new hnswlib::VisitedListPool(1, max_elements);

        this->linkLists_ = (char**)malloc(sizeof(void*) * max_elements);
        if (this->linkLists_ == nullptr)
            throw std::runtime_error(
                "Not enough memory: loadIndex failed to allocate linklists");
        this->element_levels_ = std::vector<int>(max_elements);
        this->revSize_ = 1.0 / this->mult_;
        this->ef_ = 10;
        for (size_t i = 0; i < this->cur_element_count; i++) {
            this->label_lookup_[this->getExternalLabel(i)] = i;
            unsigned int linkListSize;
            hnswlib::readBinaryPOD(input, linkListSize);
            if (linkListSize == 0) {
                this->element_levels_[i] = 0;
                this->linkLists_[i] = nullptr;
            } else {
                this->element_levels_[i] =
                    linkListSize / this->size_links_per_element_;
                this->linkLists_[i] = (char*)malloc(linkListSize);
                if (this->linkLists_[i] == nullptr)
                    throw std::runtime_error(
                        "Not enough memory: loadIndex failed to allocate "
                        "linklist");
                input.read(this->linkLists_[i], linkListSize);
            }
        }

        for (size_t i = 0; i < this->cur_element_count; i++) {
            if (this->isMarkedDeleted(i)) {
                this->num_deleted_ += 1;
                if (this->allow_replace_deleted_)
                    this->deleted_elements.insert(i);
            }
        }

        input.close();

        return;
    }
};

int main() {
    //读取文件
    std::ifstream file("../data/output.txt");
    if (file.fail()) {
        throw std::runtime_error("failed to open file");
    }

    std::vector<Item> items;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string index, key, embedding;
        if (std::getline(iss, index, ':') && std::getline(iss, key, ':') &&
            std::getline(iss, embedding)) {

            Item item;
            item.index = std::stoi(index);
            item.key = key;
            item.embedding = std::vector<float>();
            std::istringstream iss2(embedding);
            std::string embedding_value;
            while (std::getline(iss2, embedding_value, ',')) {
                item.embedding.push_back(std::stof(embedding_value));
            }
            normalize(item.embedding);
            items.push_back(item);
        }
    }

    std::cout << "items size: " << items.size() << std::endl;

    int dim = 1536;         // 定义元素的维度
    int max_elements = 200; // 定义最大元素数量，这个值应该预先知道
    int M = 16; // 与数据的内部维度度紧密相关，严重影响内存消耗
    int ef_construction = 200; // 控制索引搜索速度和构建速度之间的权衡

    hnswlib::InnerProductSpace space(dim);

    MyHNSW<float> alg_hnsw(&space, max_elements, M, ef_construction);

    for (const auto& item : items) {
        alg_hnsw.addPoint(&item.embedding[0], item.index);
    }

    //查询
    auto queryItem = items[4];
    std::cout << "query item: " << queryItem.index << queryItem.key
              << std::endl;

    normalize(queryItem.embedding);

    std::priority_queue<std::pair<float, hnswlib::labeltype>> result =
        alg_hnsw.searchKnn(&queryItem.embedding[0], 10);

    std::vector<std::pair<float, hnswlib::labeltype>> vec;

    while (!result.empty()) {
        vec.push_back(result.top());
        result.pop();
    }

    std::cout << "search result: " << std::endl;
    for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
        std::cout << it->first << " " << it->second << " "
                  << items[it->second - 1].key << std::endl;
    }

    alg_hnsw.newSaveIndex("../data/index.bin");

    alg_hnsw.newLoadIndex("../data/index.bin", &space);

    return 0;
}