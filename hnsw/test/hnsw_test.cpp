#include "hnsw.h"

#include <cstddef>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>

class HNSWTest : public ::testing::Test {
  protected:
    HNSW<float>* hnsw;
    hnswlib::InnerProductSpace* space;

    void SetUp() override {

        int dim = 3;            // 定义元素的维度
        int max_elements = 200; // 定义最大元素数量，这个值应该预先知道
        int M = 16; // 与数据的内部维度度紧密相关，严重影响内存消耗
        int ef_construction = 200; // 控制索引搜索速度和构建速度之间的权衡

        space = new hnswlib::InnerProductSpace(dim);
        hnsw = new HNSW<float>(space, max_elements, M, ef_construction);
    }

    void TearDown() override { delete hnsw; }
};

TEST_F(HNSWTest, AddPoint) {

    std::vector<float> data_point = {-0.00046821852, -0.033301666, 0.010647987};
    hnsw->addPoint(data_point.data(), 1);
    EXPECT_EQ(hnsw->getCurrentElementCount(), 1);

    std::vector<float> data_point2 = {-0.017229753, 0.0005879653, -0.01069623};
    hnsw->addPoint(data_point2.data(), 2);
    EXPECT_EQ(hnsw->getCurrentElementCount(), 2);
}

TEST_F(HNSWTest, GetPoint) {
    std::vector<int> data_point = {1, 2, 3};
    hnsw->addPoint(data_point.data(), 1);
    auto result = hnsw->getDataByLabel<int>(1);
    EXPECT_EQ(result, data_point);
};

TEST_F(HNSWTest, DeletePoint) {

    ASSERT_THROW(hnsw->markDelete(1), std::runtime_error);

    std::vector<float> data_point = {1.0, 2.0, 3.0};
    hnsw->addPoint(data_point.data(), 1);
    EXPECT_EQ(hnsw->getCurrentElementCount(), 1);

    hnsw->markDelete(1);
    EXPECT_EQ(hnsw->getDeletedCount(), 1);
    EXPECT_EQ(hnsw->getCurrentElementCount(), 1);
};

TEST_F(HNSWTest, UpdatePoint) {
    std::vector<float> data_point0 = {1, 2, 3};
    hnsw->addPoint(data_point0.data(), 2);

    std::vector<float> data_point1 = {1, 2, 3};
    hnsw->addPoint(data_point1.data(), 3);

    // get internal id by label
    auto result = hnsw->label_lookup_.find(3);
    std::vector<float> data_point2 = {4, 5, 6};
    hnsw->updatePoint(data_point2.data(), result->second, 1.0);

    EXPECT_EQ(hnsw->getCurrentElementCount(), 2);
    EXPECT_EQ(hnsw->getDeletedCount(), 0);

    std::priority_queue<std::pair<float, hnswlib::labeltype>> search =
        hnsw->searchKnn(data_point2.data(), 1);

    EXPECT_EQ(search.size(), 1);
    EXPECT_EQ(search.top().second, 3);
    EXPECT_LE(search.top().first, 0.01);
};

TEST_F(HNSWTest, UpdatePointBySameLabel) {
    std::vector<float> data_point0 = {1, 2, 3};
    hnsw->addPoint(data_point0.data(), 10);
    auto result = hnsw->getDataByLabel<float>(10);

    std::vector<float> data_point1 = {4, 5, 6};
    hnsw->addPoint(data_point1.data(), 2);
    auto result2 = hnsw->getDataByLabel<float>(2);

    EXPECT_EQ(result2, data_point1);
};