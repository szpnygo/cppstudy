#include "rookie_db.h"
#include "vector_database.h"

#include <gtest/gtest.h>
#include <iostream>
#include <memory>
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
        std::shared_ptr<TableSchema> schema = std::make_shared<TableSchema>();
        schema->addString("test");
        db->createTable("test", schema, 5, 1000, 16, 200, true);
    }
};

TEST_F(RookieDBTest, CreateTable) {
    std::shared_ptr<TableSchema> schema = std::make_shared<TableSchema>();
    db->createTable("test", schema, 5, 1000, 16, 200, false);
    ASSERT_TRUE(db->hasTable("test"));
    ASSERT_FALSE(db->hasTable("test2"));
}

TEST_F(RookieDBWithTableTest, AddAndGet) {
    std::vector<float> v = {1, 2, 3, 4, 5};
    VecData data(1, v);
    data.setAttribute("test", Value("123"));
    db->add("test", data);
    ASSERT_EQ(db->count("test"), 1);

    auto result = db->get("test", 1);
    ASSERT_EQ(result.id, 1);
    ASSERT_TRUE(result.getAttributeAs<std::string>("test").has_value());
    ASSERT_FALSE(result.getAttributeAs<int>("test").has_value());
    ASSERT_EQ(std::get<std::string>(result.attributes->at("test")), "123");
}

TEST_F(RookieDBWithTableTest, AddWithDim) {
    std::shared_ptr<TableSchema> schema = std::make_shared<TableSchema>();
    db->createTable("test2", schema, 5, 1000, 16, 200, true);
    std::vector<float> v = {1, 2, 3};
    VecData data(1, v);
    ASSERT_THROW(db->add("test2", data), std::runtime_error);

    std::vector<float> v2 = {1, 2, 3, 4, 5};
    VecData data2(1, v2);
    db->add("test2", data2);
    ASSERT_EQ(db->count("test2"), 1);
}

TEST_F(RookieDBWithTableTest, AddWithSchema) {
    std::shared_ptr<TableSchema> schema = std::make_shared<TableSchema>();
    schema->addString("string");
    schema->addInt("int");
    schema->addFloat("float");
    schema->addLong("long");
    schema->addUint64("uint64");

    db->createTable("test", schema, 5, 1000, 16, 200, true);

    std::vector<float> v = {1, 2, 3, 4, 5};
    VecData data(1, v);
    data.setAttribute("string", Value("123"));
    data.setAttribute("int", Value(123));
    data.setAttribute("float", Value(123.0f));
    data.setAttribute("long", Value(123L));
    data.setAttribute("uint64", Value(uint64_t(1)));

    db->add("test", data);
    ASSERT_EQ(db->count("test"), 1);

    VecData data2(2, v);
    data2.setAttribute("string", 123);
    data2.setAttribute("int", Value(123));
    data2.setAttribute("float", Value(123.0f));
    data2.setAttribute("long", Value(123L));
    data2.setAttribute("uint64", Value(uint64_t(1)));
    ASSERT_THROW(db->add("test", data2), std::runtime_error);
}

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
}

TEST_F(RookieDBTest, CreateEmptyTable) {
    std::shared_ptr<TableSchema> schema = std::make_shared<TableSchema>();
    db->createTable("test_empty", schema, 5, 1000, 16, 200, false);
    ASSERT_TRUE(db->hasTable("test_empty"));
}

TEST_F(RookieDBWithTableTest, AddWithWrongAttributeType) {
    std::vector<float> v = {1, 2, 3, 4, 5};
    VecData data(1, v);
    data.setAttribute("test", Value(123)); // wrong type, expected string
    ASSERT_THROW(db->add("test", data), std::runtime_error);
}

TEST_F(RookieDBWithTableTest, SearchWithNegativeCount) {
    std::vector<float> v = {1, 2, 3, 4, 5};
    ASSERT_THROW(db->search("test", v, -1), std::runtime_error);
}

TEST_F(RookieDBWithTableTest, SearchWithNonexistentKey) {
    std::vector<float> v = {1, 2, 3, 4, 5};
    ASSERT_THROW(db->search("nonexistent", v, 1), std::runtime_error);
}

TEST_F(RookieDBWithTableTest, LargeScale) {
    db->resize("test", 1000);
    for (int i = 0; i < 1000; i++) {
        std::vector<float> v = {1, 2, 3, 4, 5};
        VecData data(i, v);
        data.setAttribute("test", Value("123"));
        db->add("test", data);
    }

    ASSERT_EQ(db->count("test"), 1000);

    std::vector<float> search = {1, 2, 3, 4, 5};
    auto result = db->search("test", search, 10);
    ASSERT_EQ(result.size(), 10);
}

TEST_F(RookieDBTest, AllInOne) {
    // create schema
    std::shared_ptr<TableSchema> schema = std::make_shared<TableSchema>();
    schema->addString("username");
    schema->addInt("uid");

    // create table
    size_t dim = 32;
    size_t max_elements = 1000;
    db->createTable("user", schema, dim, max_elements, 16, 200, true);

    ASSERT_TRUE(db->hasTable("user"));

    std::mt19937 gen(99);
    std::uniform_real_distribution<> dis(0, 1);

    // init some data
    std::vector<std::vector<float>> data;
    std::vector<std::vector<float>> origin_data;
    for (size_t i = 0; i < max_elements; i++) {
        std::vector<float> v(dim);
        std::vector<float> v2(dim);
        for (size_t j = 0; j < dim; j++) {
            v[j] = dis(gen);
            v2[j] = dis(gen);
        }
        data.push_back(v);
        origin_data.push_back(v2);
    }

    // add error data with wrong dim
    std::vector<float> v(dim - 1);
    VecData data_error(1, v);
    ASSERT_THROW(db->add("user", data_error), std::runtime_error);

    // add error data with wrong attribute
    std::vector<float> v2(dim);
    VecData data_error2(1, v2);
    data_error2.setAttribute("error", Value("error"));
    ASSERT_THROW(db->add("user", data_error2), std::runtime_error);

    // add data
    for (size_t i = 0; i < max_elements; i++) {
        VecData v(i, data[i]);
        v.setAttribute("username", Value("user" + std::to_string(i)));
        v.setAttribute("uid", Value(int(i)));
        db->add("user", v);
    }

    ASSERT_EQ(db->count("user"), max_elements);

    // get data and check the result
    auto result = db->get("user", 5);
    ASSERT_EQ(result.id, 5);
    ASSERT_EQ(std::get<std::string>(result.attributes->at("username")),
              "user5");
    ASSERT_EQ(std::get<int>(result.attributes->at("uid")), 5);
}