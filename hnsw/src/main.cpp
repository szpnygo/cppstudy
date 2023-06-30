#include "hnsw.h"
#include "hnswlib/hnswalg.h"
#include "hnswlib/space_ip.h"
#include "hnswlib/space_l2.h"
#include "utils.h"

#include <fstream>
#include <hnswlib/hnswlib.h>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

struct Item {
    int index;
    std::string key;
    std::vector<float> embedding;
};

int main() {
    // 读取文件
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

    HNSW<float> alg_hnsw(&space, max_elements, M, ef_construction);

    for (const auto& item : items) {
        alg_hnsw.addPoint(&item.embedding[0], item.index);
    }

    // 查询
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