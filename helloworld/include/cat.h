#pragma once

#include "animal.h"

class Cat : public Animal {
public:
    Cat(const std::string& name);
    void speak() const override; // 覆盖基类的虚函数
};
