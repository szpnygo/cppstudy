#pragma once

#include <cmath>
#include <iostream>
#include <vector>

template <typename T>
static void writeBinaryPOD(std::ostream& out, const T& podRef) {
    out.write((char*)&podRef, sizeof(T));
}

template <typename T>
static void readBinaryPOD(std::istream& in, T& podRef) {
    in.read((char*)&podRef, sizeof(T));
}

static void normalize(std::vector<float>& v) {
    float sum = 0;
    for (const auto& val : v) {
        sum += val * val;
    }
    float norm = std::sqrt(sum);
    for (auto& val : v) {
        val /= norm;
    }
}