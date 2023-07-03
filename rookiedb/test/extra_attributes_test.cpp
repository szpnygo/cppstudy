#include "extra_attributes.h"

#include <gtest/gtest.h>
#include <memory>
#include <thread>

class ExtraAttributesTest : public ::testing::Test {
  protected:
    ExtraAttributes store;
};

TEST_F(ExtraAttributesTest, CreateAndAdd) {
    // 创建表并添加一些数据
    store.create("test_table");
    auto attr = std::make_unique<Attributes>();
    (*attr)["attr1"] = 1;
    (*attr)["attr2"] = 2.0f;
    (*attr)["attr3"] = 3l;
    (*attr)["attr4"] = 4ull;
    (*attr)["attr5"] = "test";
    store.add("test_table", 1, std::move(attr));

    // 读取并验证数据
    auto attr_read = store.Get("test_table", 1);
    ASSERT_TRUE(attr_read != nullptr);
    ASSERT_EQ(std::get<int>(attr_read->at("attr1")), 1);
    ASSERT_EQ(std::get<float>(attr_read->at("attr2")), 2.0f);
    ASSERT_EQ(std::get<long>(attr_read->at("attr3")), 3l);
    ASSERT_EQ(std::get<uint64_t>(attr_read->at("attr4")), 4ull);
    ASSERT_EQ(std::get<std::string>(attr_read->at("attr5")), "test");
}

TEST_F(ExtraAttributesTest, Erase) {
    // 创建表并添加一些数据
    store.create("test_table");
    auto attr = std::make_unique<Attributes>();
    (*attr)["attr1"] = 1;
    store.add("test_table", 1, std::move(attr));

    // 删除数据
    store.erase("test_table", 1);

    // 验证数据已经被删除
    ASSERT_EQ(store.Get("test_table", 1), nullptr);
}

TEST_F(ExtraAttributesTest, HighConcurrency) {
    const int kNumThreads = 100;
    const int kNumOperations = 1000;
    const int kNumTables = 10;

    // 创建多个表
    for (int i = 0; i < kNumTables; ++i) {
        store.create("table" + std::to_string(i));
    }

    std::vector<std::thread> threads;
    for (int i = 0; i < kNumThreads; ++i) {
        threads.push_back(std::thread([&, i] {
            // 写入数据
            for (int j = 0; j < kNumOperations; ++j) {
                auto attr = std::make_unique<Attributes>();
                (*attr)["attr"] = int(i * kNumOperations + j);
                store.add("table" + std::to_string(i % kNumTables),
                          i * kNumOperations + j,
                          std::move(attr));
            }

            // 读取并验证数据
            for (int j = 0; j < kNumOperations; ++j) {
                auto attr_read =
                    store.Get("table" + std::to_string(i % kNumTables),
                              i * kNumOperations + j);
                ASSERT_TRUE(attr_read != nullptr);
                ASSERT_EQ(std::get<int>(attr_read->at("attr")),
                          i * kNumOperations + j);
            }
        }));
    }

    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }
}

TEST_F(ExtraAttributesTest, ConcurrencyOnSameKey) {
    const int kNumThreads = 1000; // 线程数
    store.create("test_table");

    // 启动多个线程并发执行写入和读取操作
    std::vector<std::thread> threads;
    for (int i = 0; i < kNumThreads; ++i) {
        threads.push_back(std::thread([this, i, kNumThreads] {
            // 写入数据
            auto attr = std::make_unique<Attributes>();
            (*attr)["attr"] = i;
            store.add("test_table", 1, std::move(attr));

            // 读取并验证数据
            auto attr_read = store.Get("test_table", 1);
            ASSERT_TRUE(attr_read != nullptr);
            // 由于是并发环境，我们无法保证读取到的值就是这个线程写入的值，因此这里只能检查读取到的值在预期的范围内
            ASSERT_GE(std::get<int>(attr_read->at("attr")), 0);
            ASSERT_LT(std::get<int>(attr_read->at("attr")), kNumThreads);
        }));
    }

    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }
}
