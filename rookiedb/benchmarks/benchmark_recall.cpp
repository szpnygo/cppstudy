#include "vector_database.h"

#include <iostream>
#include <random>
#include <vector>

struct Params {
    int top;
    int dim;
    int max;
    int m;
    int ef;
    bool normalize;
};

int main() {
    std::vector<Params> params = {
        {1, 32, 200, 16, 200, false},   {1, 32, 200, 16, 500, false},
        {1, 32, 200, 32, 200, false},   {1, 32, 200, 32, 500, false},
        {1, 32, 200, 64, 200, false},   {1, 32, 200, 64, 500, false},
        {1, 32, 200, 96, 200, false},   {1, 32, 200, 96, 500, false},
        {1, 32, 1000, 16, 200, false},  {1, 32, 1000, 16, 500, false},
        {1, 32, 1000, 32, 200, false},  {1, 32, 1000, 32, 500, false},
        {1, 32, 1000, 64, 200, false},  {1, 32, 1000, 64, 500, false},
        {1, 32, 1000, 96, 200, false},  {1, 32, 1000, 96, 500, false},
        {1, 256, 200, 16, 200, false},  {1, 256, 200, 16, 500, false},
        {1, 256, 200, 32, 200, false},  {1, 256, 200, 32, 500, false},
        {1, 256, 200, 64, 200, false},  {1, 256, 200, 64, 500, false},
        {1, 256, 200, 96, 200, false},  {1, 256, 200, 96, 500, false},
        {1, 256, 1000, 16, 200, false}, {1, 256, 1000, 16, 500, false},
        {1, 256, 1000, 32, 200, false}, {1, 256, 1000, 32, 500, false},
        {1, 256, 1000, 64, 200, false}, {1, 256, 1000, 64, 500, false},
        {1, 256, 1000, 96, 200, false}, {1, 256, 1000, 96, 500, false},
        {1, 32, 200, 16, 200, true},    {1, 32, 200, 16, 500, true},
        {1, 32, 200, 32, 200, true},    {1, 32, 200, 32, 500, true},
        {1, 32, 200, 64, 200, true},    {1, 32, 200, 64, 500, true},
        {1, 32, 200, 96, 200, true},    {1, 32, 200, 96, 500, true},
        {1, 32, 1000, 16, 200, true},   {1, 32, 1000, 16, 500, true},
        {1, 32, 1000, 32, 200, true},   {1, 32, 1000, 32, 500, true},
        {1, 32, 1000, 64, 200, true},   {1, 32, 1000, 64, 500, true},
        {1, 32, 1000, 96, 200, true},   {1, 32, 1000, 96, 500, true},
        {1, 256, 200, 16, 200, true},   {1, 256, 200, 16, 500, true},
        {1, 256, 200, 32, 200, true},   {1, 256, 200, 32, 500, true},
        {1, 256, 200, 64, 200, true},   {1, 256, 200, 64, 500, true},
        {1, 256, 200, 96, 200, true},   {1, 256, 200, 96, 500, true},
        {1, 256, 1000, 16, 200, true},  {1, 256, 1000, 16, 500, true},
        {1, 256, 1000, 32, 200, true},  {1, 256, 1000, 32, 500, true},
        {1, 256, 1000, 64, 200, true},  {1, 256, 1000, 64, 500, true},
        {1, 256, 1000, 96, 200, true},  {1, 256, 1000, 96, 500, true},
    };

    std::mt19937 gen(99);
    std::uniform_real_distribution<> dis(0, 1.0);

    for (auto p : params) {
        auto start = std::chrono::high_resolution_clock::now();

        VectorDatabase db("test", p.dim, p.max, p.m, p.ef, p.normalize);
        std::vector<std::vector<float>> data;
        for (int i = 0; i < p.max; ++i) {
            std::vector<float> vec(p.dim);
            for (int j = 0; j < p.dim; ++j) {
                vec[j] = dis(gen);
            }
            data.push_back(vec);
            db.add(i, data[i]);
        }

        float correct = 0;
        for (int i = 0; i < p.max; ++i) {
            auto result = db.search(data[i], 10);
            for (int j = 0; j < p.top; ++j) {
                if (result[j].first == i) {
                    correct++;
                    break;
                }
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;

        std::cout << "top: " << p.top << " dim: " << p.dim << " max: " << p.max
                  << " m: " << p.m << " ef: " << p.ef
                  << " normalize: " << p.normalize
                  << " recall: " << correct / p.max << " time: " << diff.count()
                  << std::endl;
    }
}