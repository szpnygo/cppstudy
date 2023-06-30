#pragma once

#include "hnswlib/hnswalg.h"
#include "utils.h"

template <typename dist_t>
class HNSW : public hnswlib::HierarchicalNSW<dist_t> {
  public:
    using hnswlib::HierarchicalNSW<dist_t>::HierarchicalNSW;

    void newSaveIndex(const std::string& location) {
        std::ofstream output(location, std::ios::binary);
        std::streampos position;

        writeBinaryPOD(output, "aaaaaaa");

        writeBinaryPOD(output, this->offsetLevel0_);
        writeBinaryPOD(output, this->max_elements_);
        writeBinaryPOD(output, this->cur_element_count);
        writeBinaryPOD(output, this->size_data_per_element_);
        writeBinaryPOD(output, this->label_offset_);
        writeBinaryPOD(output, this->offsetData_);
        writeBinaryPOD(output, this->maxlevel_);
        writeBinaryPOD(output, this->enterpoint_node_);
        writeBinaryPOD(output, this->maxM_);

        writeBinaryPOD(output, this->maxM0_);
        writeBinaryPOD(output, this->M_);
        writeBinaryPOD(output, this->mult_);
        writeBinaryPOD(output, this->ef_construction_);

        output.write(this->data_level0_memory_,
                     this->cur_element_count * this->size_data_per_element_);

        for (size_t i = 0; i < this->cur_element_count; i++) {
            unsigned int linkListSize =
                this->element_levels_[i] > 0
                    ? this->size_links_per_element_ * this->element_levels_[i]
                    : 0;
            writeBinaryPOD(output, linkListSize);
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
        readBinaryPOD(input, buffer);
        std::cout << buffer << std::endl;
        readBinaryPOD(input, this->offsetLevel0_);
        readBinaryPOD(input, this->max_elements_);
        readBinaryPOD(input, this->cur_element_count);

        size_t max_elements = max_elements_i;
        if (max_elements < this->cur_element_count)
            max_elements = this->max_elements_;
        this->max_elements_ = max_elements;
        readBinaryPOD(input, this->size_data_per_element_);
        readBinaryPOD(input, this->label_offset_);
        readBinaryPOD(input, this->offsetData_);
        readBinaryPOD(input, this->maxlevel_);
        readBinaryPOD(input, this->enterpoint_node_);

        readBinaryPOD(input, this->maxM_);
        readBinaryPOD(input, this->maxM0_);
        readBinaryPOD(input, this->M_);
        readBinaryPOD(input, this->mult_);
        readBinaryPOD(input, this->ef_construction_);

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
            readBinaryPOD(input, linkListSize);
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
            readBinaryPOD(input, linkListSize);
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