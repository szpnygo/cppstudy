#include "vector_database.h"

#include <cstddef>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <vector>

class VectorDatabaseTest : public ::testing::Test {
  protected:
    VectorDatabase* db;

    virtual void SetUp() override {
        db = new VectorDatabase("test", 5, 1000, 16, 200, false);
    }

    virtual void TearDown() override { delete db; }
};

class VectorDatabaseNormalize : public ::testing::Test {
  protected:
    VectorDatabase* db;

    virtual void SetUp() override {
        db = new VectorDatabase("test", 3, 1000, 16, 200, true);
    }

    virtual void TearDown() override { delete db; }
};

TEST_F(VectorDatabaseTest, Init) { ASSERT_EQ(db->name(), "test"); }

TEST_F(VectorDatabaseTest, Add) {
    std::vector<float> data = {1, 2, 3, 4, 5};
    db->add(1, data);
    ASSERT_EQ(db->count(), 1);
}

TEST_F(VectorDatabaseTest, Del) {
    std::vector<float> data = {1, 2, 3, 4, 5};
    db->add(1, data);
    ASSERT_EQ(db->count(), 1);

    std::vector<float> data2 = {1, 2, 3, 4, 5};
    db->add(2, data2);
    ASSERT_EQ(db->count(), 2);

    db->del(1);
    ASSERT_EQ(db->count(), 1);

    db->del(2);
    ASSERT_EQ(db->count(), 0);

    ASSERT_EQ(db->count_deleted(), 2);
}

TEST_F(VectorDatabaseTest, Get) {
    std::vector<float> data = {1, 2, 3, 4, 5};
    db->add(1, data);
    auto result = db->get(1);
    ASSERT_EQ(result.size(), 5);
    ASSERT_EQ(result[0], 1);
    ASSERT_EQ(result[1], 2);
    ASSERT_EQ(result[2], 3);
    ASSERT_EQ(result[3], 4);
    ASSERT_EQ(result[4], 5);
}

TEST_F(VectorDatabaseTest, Count) {
    std::vector<float> data = {1, 2, 3, 4, 5};
    db->add(1, data);
    ASSERT_EQ(db->count(), 1);

    std::vector<float> data2 = {1, 2, 3, 4, 5};
    db->add(2, data2);
    ASSERT_EQ(db->count(), 2);
}

TEST_F(VectorDatabaseTest, Exists) {
    std::vector<float> data = {1, 2, 3, 4, 5};
    db->add(1, data);
    ASSERT_TRUE(db->exists(1));
    ASSERT_FALSE(db->exists(2));
}

TEST_F(VectorDatabaseTest, Update) {
    std::vector<float> data = {1, 2, 3, 4, 5};
    db->add(1, data);
    ASSERT_EQ(db->count(), 1);

    std::vector<float> data2 = {6, 7, 8, 9, 10};
    db->update(1, data2);
    ASSERT_EQ(db->count(), 1);

    auto result = db->get(1);
    ASSERT_EQ(result.size(), 5);
    ASSERT_EQ(result[0], 6);
    ASSERT_EQ(result[1], 7);
    ASSERT_EQ(result[2], 8);
    ASSERT_EQ(result[3], 9);
    ASSERT_EQ(result[4], 10);
}

TEST_F(VectorDatabaseTest, GetInternalID) {
    std::vector<float> data = {1, 2, 3, 4, 5};
    db->add(1, data);
    ASSERT_EQ(db->getInternalID(1), 0);

    std::vector<float> data2 = {6, 7, 8, 9, 10};
    db->add(2, data2);
    ASSERT_EQ(db->getInternalID(2), 1);

    std::vector<float> data3 = {11, 12, 13, 14, 15};
    db->add(3, data3);
    ASSERT_EQ(db->getInternalID(3), 2);

    ASSERT_EQ(db->getInternalID(4), 0);

    db->del(1);
    ASSERT_EQ(db->getInternalID(1), 0);
}

TEST_F(VectorDatabaseTest, ResizeMaxElements) {
    std::vector<float> data = {1, 2, 3, 4, 5};
    db->add(1, data);

    ASSERT_EQ(db->getMaxElements(), 1000);

    db->resizeIndex(1200);

    ASSERT_EQ(db->getMaxElements(), 1200);
}

TEST_F(VectorDatabaseNormalize, Normalize) {
    VectorDatabase db("test", 5, 1000, 16, 200, true);

    std::vector<float> data = {1, 2, 3};
    db.add(1, data);

    auto result = db.get(1);
    ASSERT_EQ(result.size(), 5);

    for (auto item : data) {
        ASSERT_TRUE(item >= 0 && item <= 1);
    }

    std::vector<float> original = {1, 2, 3};
    float sum = 0;
    for (const auto& val : original) {
        sum += val * val;
    }
    float norm = std::sqrt(sum);
    for (auto& val : original) {
        val /= norm;
    }

    for (int i = 0; i < 3; i++) {
        ASSERT_EQ(result[i], original[i]);
    }
}

float recallTest(
    size_t dim, size_t maxElements, size_t m, size_t ef, bool normalize) {
    VectorDatabase db("test", dim, maxElements, m, ef, normalize);

    std::mt19937 gen(99);
    std::uniform_real_distribution<> dis(0, 1);

    std::vector<std::vector<float>> data;

    for (int i = 0; i < maxElements; ++i) {
        std::vector<float> vec(dim);
        for (int j = 0; j < dim; ++j) {
            vec[j] = dis(gen);
        }
        data.push_back(vec);
        db.add(i, data[i]);
    }
    float correct = 0;

    for (int i = 0; i < maxElements; ++i) {
        auto result = db.search(data[i], 10);
        for (int j = 0; j < 3; ++j) {
            if (result[j].first == i) {
                correct++;
                break;
            }
        }
    }

    return correct / maxElements;
}

TEST(VectorDataBase, RecallTop3_16_200) {
    auto recall = recallTest(32, 200, 16, 200, false);
    EXPECT_GE(recall, 0.8);
}