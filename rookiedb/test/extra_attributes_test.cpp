#include "extra_attributes.h"

#include <gtest/gtest.h>
#include <thread>

class ExtraAttributesTest : public ::testing::Test {
  protected:
    ExtraAttributes store;
};

TEST_F(ExtraAttributesTest, Insert) {
    store.insert("key", "subkey", 1);
    auto result = store.get("key", "subkey");
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(std::get<int>(result.value()), 1);

    store.insert("key", "subkey2", "value");
    auto result2 = store.get("key", "subkey2");
    ASSERT_TRUE(result2.has_value());
    ASSERT_EQ(std::get<std::string>(result2.value()), "value");

    auto result3 = store.get("key", "subkey3");
    ASSERT_FALSE(result3.has_value());

    store.insert("key2", "subkey", 1);
    auto result4 = store.get("key2", "subkey");
    ASSERT_TRUE(result4.has_value());
}

TEST_F(ExtraAttributesTest, Del) {
    store.insert("key", "subkey", 1);
    store.erase("key", "subkey");
    auto result = store.get("key", "subkey");
    ASSERT_FALSE(result.has_value());
}

TEST_F(ExtraAttributesTest, MultiThreadedOperationsSameKey) {
    std::thread t1([&]() { store.insert("key1", "key2", "value1"); });
    std::thread t2([&]() { store.insert("key1", "key3", "value2"); });

    t1.join();
    t2.join();

    auto value1 = store.get("key1", "key2");
    ASSERT_TRUE(value1.has_value());
    ASSERT_EQ(std::get<std::string>(value1.value()), "value1");

    auto value2 = store.get("key1", "key3");
    ASSERT_TRUE(value2.has_value());
    ASSERT_EQ(std::get<std::string>(value2.value()), "value2");
}

TEST_F(ExtraAttributesTest, MultiThreadedOperationDifferentKey) {
    std::thread t1([&]() { store.insert("key1", "key2", "value1"); });
    std::thread t2([&]() { store.insert("key2", "key3", "value2"); });

    t1.join();
    t2.join();

    auto value1 = store.get("key1", "key2");
    ASSERT_TRUE(value1.has_value());
    ASSERT_EQ(std::get<std::string>(value1.value()), "value1");

    auto value2 = store.get("key2", "key3");
    ASSERT_TRUE(value2.has_value());
    ASSERT_EQ(std::get<std::string>(value2.value()), "value2");
}