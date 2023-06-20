#pragma once

#include <string>

class Animal {
public:
    Animal(const std::string& name);
    virtual ~Animal();
    virtual void speak() const = 0; // 纯虚函数，使Animal成为一个抽象类

protected:
    std::string name;
};
