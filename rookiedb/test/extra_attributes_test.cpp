#include "extra_attributes.h"

#include <gtest/gtest.h>
#include <thread>

class ExtraAttributesTest : public ::testing::Test {
  protected:
    ExtraAttributes store;
};

// Test for insert and get methods
TEST_F(ExtraAttributesTest, InsertAndGet) {
    store.insert(123, "foo", "bar");

    std::optional<Value> value = store.get(123, "foo");

    ASSERT_TRUE(value.has_value());
    ASSERT_EQ(std::get<std::string>(*value), "bar");
}

// Test for erase method
TEST_F(ExtraAttributesTest, Erase) {
    store.insert(123, "foo", "bar");
    store.erase(123, "foo");

    std::optional<Value> value = store.get(123, "foo");

    ASSERT_FALSE(value.has_value());
}

// Test for concurrent inserts with more threads
TEST_F(ExtraAttributesTest, ConcurrentInsert) {
    const int num_threads = 100;
    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; i++) {
        threads.push_back(std::thread([&, i]() {
            store.insert(
                123, "key" + std::to_string(i), "value" + std::to_string(i));
        }));
    }

    for (auto& t : threads) {
        t.join();
    }

    for (int i = 0; i < num_threads; i++) {
        std::optional<Value> value = store.get(123, "key" + std::to_string(i));
        ASSERT_TRUE(value.has_value());
        ASSERT_EQ(std::get<std::string>(*value), "value" + std::to_string(i));
    }
}

// Test for concurrent inserts and deletes
TEST_F(ExtraAttributesTest, ConcurrentInsertAndDelete) {
    const int num_threads = 100;
    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; i++) {
        if (i % 2 == 0) {
            threads.push_back(std::thread([&, i]() {
                store.insert(123,
                             "key" + std::to_string(i),
                             "value" + std::to_string(i));
            }));
        } else {
            threads.push_back(std::thread(
                [&, i]() { store.erase(123, "key" + std::to_string(i)); }));
        }
    }

    for (auto& t : threads) {
        t.join();
    }

    for (int i = 0; i < num_threads; i++) {
        std::optional<Value> value = store.get(123, "key" + std::to_string(i));
        if (i % 2 == 0) {
            ASSERT_TRUE(value.has_value());
            ASSERT_EQ(std::get<std::string>(*value),
                      "value" + std::to_string(i));
        } else {
            ASSERT_FALSE(value.has_value());
        }
    }
}

// Test for concurrent inserts on the same sub-key
TEST_F(ExtraAttributesTest, ConcurrentInsertSameSubKey) {
    const int num_threads = 100;
    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; i++) {
        threads.push_back(std::thread(
            [&, i]() { store.insert(i, "key", "value" + std::to_string(i)); }));
    }

    for (auto& t : threads) {
        t.join();
    }

    for (int i = 0; i < num_threads; i++) {
        std::optional<Value> value = store.get(i, "key");
        ASSERT_TRUE(value.has_value());
        ASSERT_EQ(std::get<std::string>(*value), "value" + std::to_string(i));
    }
}

// Test for concurrent inserts on different sub-keys
TEST_F(ExtraAttributesTest, ConcurrentInsertDifferentSubKeys) {
    const int num_threads = 100;
    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; i++) {
        threads.push_back(std::thread([&, i]() {
            store.insert(
                123, "key" + std::to_string(i), "value" + std::to_string(i));
        }));
    }

    for (auto& t : threads) {
        t.join();
    }

    for (int i = 0; i < num_threads; i++) {
        std::optional<Value> value = store.get(123, "key" + std::to_string(i));
        ASSERT_TRUE(value.has_value());
        ASSERT_EQ(std::get<std::string>(*value), "value" + std::to_string(i));
    }
}