#include "hnsw.h"

#include <gtest/gtest.h>

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
    hnsw->addPoint(&data_point, 1);
    EXPECT_EQ(hnsw->getCurrentElementCount(), 1);

    std::vector<float> data_point2 = {-0.017229753, 0.0005879653, -0.01069623};
    hnsw->addPoint(&data_point2, 2);
    EXPECT_EQ(hnsw->getCurrentElementCount(), 2);
}

TEST_F(HNSWTest, DeletePoint) {

    ASSERT_THROW(hnsw->markDelete(1), std::runtime_error);

    std::vector<float> data_point = {1.0, 2.0, 3.0};
    hnsw->addPoint(&data_point, 1);
    EXPECT_EQ(hnsw->getCurrentElementCount(), 1);

    hnsw->markDelete(1);
    EXPECT_EQ(hnsw->getDeletedCount(), 1);
    EXPECT_EQ(hnsw->getCurrentElementCount(), 1);
};