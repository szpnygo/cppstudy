#include "rookie_db.h"
#include "vector_database.h"

#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <vector>

class RookieDBTest : public ::testing::Test {
  protected:
    RookieDB* db;

    std::shared_ptr<Filter> filter;

    virtual void SetUp() override {
        db = new RookieDB();
        std::shared_ptr<TableSchema> schema = std::make_shared<TableSchema>();
        schema->addString("username");
        schema->addInt("uid");
        schema->addFloat("vec");
        db->createTable("users", schema, 5, 200);

        for (int i = 0; i < 100; i++) {
            std::vector<float> v = {1, 2, 3, 4, 5};
            VecData data(i, v);
            data.setAttribute("username", Value("user" + std::to_string(i)));
            data.setAttribute("uid", Value(i));
            db->add("users", data);
        }

        filter = std::make_shared<Filter>();
    }

    virtual void TearDown() override { delete db; }
};

TEST_F(RookieDBTest, SearchWithFilterEQ) {
    filter->setEqual("uid", Value(3));

    std::vector<float> search = {1, 2, 3, 4, 5};
    auto result = db->search("users", search, 5, filter);
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0].first, 3);
}

TEST_F(RookieDBTest, SearchWithFilterNE) {
    filter->setNotEqual("uid", Value(3));

    std::vector<float> search = {1, 2, 3, 4, 5};
    auto result = db->search("users", search, 100, filter);
    ASSERT_EQ(result.size(), 99);
}

TEST_F(RookieDBTest, SearchWithFilterLT) {
    filter->setLessThan("uid", Value(3));

    std::vector<float> search = {1, 2, 3, 4, 5};
    auto result = db->search("users", search, 5, filter);
    ASSERT_EQ(result.size(), 3);
}

TEST_F(RookieDBTest, SearchWithFilterLE) {
    filter->setLessThanOrEqual("uid", Value(3));

    std::vector<float> search = {1, 2, 3, 4, 5};
    auto result = db->search("users", search, 5, filter);
    ASSERT_EQ(result.size(), 4);
}

TEST_F(RookieDBTest, SearchWithFilterGT) {
    filter->setGreaterThan("uid", Value(3));

    std::vector<float> search = {1, 2, 3, 4, 5};
    auto result = db->search("users", search, 100, filter);
    ASSERT_EQ(result.size(), 96);
}

TEST_F(RookieDBTest, SearchWithFilterGE) {
    filter->setGreaterThanOrEqual("uid", Value(3));

    std::vector<float> search = {1, 2, 3, 4, 5};
    auto result = db->search("users", search, 100, filter);
    ASSERT_EQ(result.size(), 97);
}

TEST_F(RookieDBTest, SearchWithErrorFilter) {
    filter->setEqual("uid", "3");

    std::vector<float> search = {1, 2, 3, 4, 5};
    ASSERT_THROW(db->search("users", search, 5, filter), std::runtime_error);
}

TEST_F(RookieDBTest, SearchWithFilterAnd) {
    filter->setEqual("uid", Value(3));
    filter->setEqual("username", Value("user3"));

    std::vector<float> search = {1, 2, 3, 4, 5};
    auto result = db->search("users", search, 5, filter);
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0].first, 3);
}