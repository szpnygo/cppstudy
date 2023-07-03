#include "rookie_db.h"
#include "vector_database.h"

#include <gtest/gtest.h>
#include <iostream>
#include <vector>

class RookieDBTest : public ::testing::Test {
  protected:
    RookieDB* db;

    virtual void SetUp() override { db = new RookieDB(); }

    virtual void TearDown() override { delete db; }
};

class RookieDBWithTableTest : public RookieDBTest {
  protected:
    virtual void SetUp() override {
        RookieDBTest::SetUp();
        db->createTable("test", 5, 1000, 16, 200, true);
    }
};

TEST_F(RookieDBTest, CreateTable) {
    db->createTable("test", 5, 1000, 16, 200, false);
    ASSERT_TRUE(db->hasTable("test"));
    ASSERT_FALSE(db->hasTable("test2"));
}

TEST_F(RookieDBWithTableTest, Add) {
    std::vector<float> v = {1, 2, 3, 4, 5};
    VecData data(1, v);
    db->add("test", data);
    ASSERT_EQ(db->count("test"), 1);

    db->createTable("test2", 5, 200);
    std::vector<float> v3 = {1, 2, 3, 4, 5};
    VecData data3(3, v3);
    db->add("test2", data3);
    ASSERT_EQ(db->count("test2"), 1);

    std::vector<float> v2 = {1, 2, 3, 4, 5};
    VecData data2(2, v2);
    db->add("test", data2);
    ASSERT_EQ(db->count("test"), 2);
}

class MySearchFilter : public SearchFilter {
  protected:
    bool operator()(uint64_t id) { return id == 3 || id == 4; };
};

TEST_F(RookieDBWithTableTest, Search) {
    std::vector<float> v = {1, 2, 3, 4, 5};
    VecData data(12, v);
    db->add("test", data);
    std::vector<float> search = {1, 2, 3, 4, 5};
    auto result = db->search("test", search, 1);
    ASSERT_EQ(result.size(), 1);

    // add 10 random vector data
    for (int i = 0; i < 10; i++) {
        std::vector<float> v = {1, 2, 3, 4, 5};
        VecData data(i, v);
        db->add("test", data);
    }

    ASSERT_EQ(db->count("test"), 11);

    std::vector<float> search2 = {1, 2, 3, 4, 5};
    auto result2 = db->search("test", search2, 6);
    ASSERT_EQ(result2.size(), 6);

    std::vector<float> search3 = {1, 2, 3, 4, 5};
    auto result3 = db->search("test", search3, 15);
    ASSERT_EQ(result3.size(), 11);

    MySearchFilter* filter = new MySearchFilter();
    std::vector<float> search4 = {1, 2, 3, 4, 5};
    auto result4 = db->search("test", search4, 15, filter);
    ASSERT_EQ(result4.size(), 2);
}