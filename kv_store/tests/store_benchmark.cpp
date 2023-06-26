#include "hash_store.h"

#include <benchmark/benchmark.h>
#include <string>

static void BM_HashStoreSet(benchmark::State& state) {
    std::string filename = "benchmark_store_set.txt";
    HashStore store(filename);
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            store.Set(std::to_string(i), "value");
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
    std::remove(filename.c_str());
}

static void BM_HashStoreGet(benchmark::State& state) {
    std::string filename = "benchmark_store_get.txt";
    HashStore store(filename);
    for (int i = 0; i < state.range(0); ++i) {
        store.Set(std::to_string(i), "value");
    }

    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            store.Get(std::to_string(i));
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
    std::remove(filename.c_str());
}

BENCHMARK(BM_HashStoreSet)->Range(8 << 4, 8 << 12);
BENCHMARK(BM_HashStoreGet)->Range(8 << 4, 8 << 12);

BENCHMARK_MAIN();