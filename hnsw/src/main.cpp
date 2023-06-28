#include "hnswlib/hnswalg.h"
#include "hnswlib/space_ip.h"
#include "hnswlib/space_l2.h"

#include <hnswlib/hnswlib.h>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

int main() {

    int dim = 128;            // 定义元素的维度
    int max_elements = 10000; // 定义最大元素数量，这个值应该预先知道
    int M = 16; // 与数据的内部维度度紧密相关，严重影响内存消耗
    int ef_construction = 200; // 控制索引搜索速度和构建速度之间的权衡

    hnswlib::InnerProductSpace space(dim);

    hnswlib::HierarchicalNSW<float> alg_hnsw(
        &space, max_elements, M, ef_construction);

    // 生成随机数据
    std::mt19937 rng;
    rng.seed(47);                                  // 设置随机数种子
    std::uniform_real_distribution<> distrib_real; // 创建实数均匀分布生成器
    std::vector<std::vector<float>> data; // 存储所有数据的容器
    for (int i = 0; i < max_elements; ++i) {
        std::vector<float> vec(dim); // 创建一个元素（向量）
        for (int j = 0; j < dim; ++j) {
            vec[j] = distrib_real(rng); // 向量的每个维度赋予一个随机值
        }
        data.push_back(vec); // 将元素添加到数据容器中
    }

    // 将数据添加到索引中
    for (int i = 0; i < max_elements; i++) {
        alg_hnsw.addPoint(&data[i][0], i); // 将数据点添加到 HNSW 索引中
    }

    // 用自身数据查询元素并计算召回率
    float correct = 0;
    for (int i = 0; i < max_elements; i++) {
        // 执行 KNN 搜索，参数分别为数据和搜索的最近邻居数量
        std::priority_queue<std::pair<float, hnswlib::labeltype>> result =
            alg_hnsw.searchKnn(&data[i][0], 1);
        hnswlib::labeltype label = result.top().second; // 获取搜索结果的标签
        if (label == i)
            correct++; // 如果结果的标签与期望的标签一致，则正确数量加1
    }
    float recall = correct / max_elements;     // 计算召回率
    std::cout << "Recall: " << recall << "\n"; // 打印召回率

    return 0;
}