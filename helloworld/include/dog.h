#pragma once

#include "animal.h"

class Dog : public Animal {
public:
    Dog(const std::string& name);
    void speak() const override; // 覆盖基类的虚函数
};
