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
        std::shared_ptr<TableSchema> schema = std::make_shared<TableSchema>();
        db->createTable("test", schema, 5, 1000, 16, 200, true);
    }
};

TEST_F(RookieDBTest, CreateTable) {
    std::shared_ptr<TableSchema> schema = std::make_shared<TableSchema>();
    db->createTable("test", schema, 5, 1000, 16, 200, false);
    ASSERT_TRUE(db->hasTable("test"));
    ASSERT_FALSE(db->hasTable("test2"));
}

TEST_F(RookieDBWithTableTest, Add) {
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

    std::shared_ptr<TableSchema> schema = std::make_shared<TableSchema>();
    db->createTable("test2", schema, 5, 200);
    std::vector<float> v3 = {1, 2, 3, 4, 5};
    VecData data3(3, v3);
    data3.setAttribute("test", Value(1));
    db->add("test2", data3);
    ASSERT_EQ(db->count("test2"), 1);

    auto result3 = db->get("test2", 3);
    ASSERT_EQ(result3.id, 3);
    ASSERT_TRUE(result3.getAttributeAs<int>("test").has_value());
    ASSERT_FALSE(result3.getAttributeAs<float>("test").has_value());
    ASSERT_EQ(std::get<int>(result3.attributes->at("test")), 1);

    std::vector<float> v2 = {1, 2, 3, 4, 5};
    VecData data2(2, v2);
    data2.setAttribute("test", Value(1.1f));
    db->add("test", data2);
    ASSERT_EQ(db->count("test"), 2);

    auto result2 = db->get("test", 2);
    ASSERT_EQ(result2.id, 2);
    ASSERT_TRUE(result2.getAttributeAs<float>("test").has_value());
    ASSERT_FALSE(result2.getAttributeAs<int>("test").has_value());
    ASSERT_EQ(std::get<float>(result2.attributes->at("test")), 1.1f);
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