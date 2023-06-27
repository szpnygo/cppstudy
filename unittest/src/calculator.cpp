#include "calculator.h"

#include <iostream>

int Calculator::add(int a, int b) {
    return a + b; // 返回两数之和
}

int Calculator::subtract(int a, int b) {
    return a - b; // 返回两数之差
}

int Calculator::multiply(int a, int b) {
    return a * b; // 返回两数之积
}

double Calculator::divide(int a, int b) {
    if (b == 0) {
        throw std::invalid_argument("除数不能为0");
    }
    return static_cast<double>(a) / b;
}
