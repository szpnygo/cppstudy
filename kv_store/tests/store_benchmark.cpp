#include "hash_store.h"
#include "sharded_hash_store.h"

#include <benchmark/benchmark.h>
#include <random>
#include <string>
#include <thread>
#include <vector>

constexpr size_t kNumThreads = 4; // Adjust as appropriate for your hardware.

constexpr size_t kNumArgs = 2 << 18;

// Function for worker threads to run during Set benchmark.
void SetWork(benchmark::State& state,
             BaseStore& store,
             std::mt19937& gen,
             std::uniform_int_distribution<>& distrib) {
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            store.Set(std::to_string(distrib(gen)), "value");
        }
    }
}

// Function for worker threads to run during Get benchmark.
void GetWork(benchmark::State& state,
             BaseStore& store,
             std::mt19937& gen,
             std::uniform_int_distribution<>& distrib) {
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            store.Get(std::to_string(distrib(gen)));
        }
    }
}

template <typename StoreType>
static void BM_StoreSet(benchmark::State& state) {
    std::string filename = "benchmark_store_set.txt";
    StoreType store(filename);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 1 << 12);

    // Vector to hold all worker threads.
    std::vector<std::thread> workers;

    for (auto _ : state) {
        // Spawn worker threads.
        for (size_t i = 0; i < kNumThreads; ++i) {
            workers.emplace_back(SetWork,
                                 std::ref(state),
                                 std::ref(store),
                                 std::ref(gen),
                                 std::ref(distrib));
        }
        // Join worker threads.
        for (auto& worker : workers) {
            worker.join();
        }
        // Clear workers for next iteration.
        workers.clear();
    }

    state.SetItemsProcessed(state.iterations() * state.range(0) * kNumThreads);
    std::remove(filename.c_str());
}

template <typename StoreType>
static void BM_StoreGet(benchmark::State& state) {
    std::string filename = "benchmark_store_get.txt";
    StoreType store(filename);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 1 << 8);

    // Initialize the store with some data.
    for (int i = 0; i < state.range(0); ++i) {
        store.Set(std::to_string(distrib(gen)), "value");
    }

    // Vector to hold all worker threads.
    std::vector<std::thread> workers;

    for (auto _ : state) {
        // Spawn worker threads.
        for (size_t i = 0; i < kNumThreads; ++i) {
            workers.emplace_back(GetWork,
                                 std::ref(state),
                                 std::ref(store),
                                 std::ref(gen),
                                 std::ref(distrib));
        }
        // Join worker threads.
        for (auto& worker : workers) {
            worker.join();
        }
        // Clear workers for next iteration.
        workers.clear();
    }

    state.SetItemsProcessed(state.iterations() * state.range(0) * kNumThreads);
    std::remove(filename.c_str());
}

BENCHMARK_TEMPLATE(BM_StoreSet, HashStore)->Arg(kNumArgs);
BENCHMARK_TEMPLATE(BM_StoreSet, ShardedHashStore)->Arg(kNumArgs);

BENCHMARK_TEMPLATE(BM_StoreGet, HashStore)->Arg(kNumArgs);
BENCHMARK_TEMPLATE(BM_StoreGet, ShardedHashStore)->Arg(kNumArgs);

BENCHMARK_MAIN();