#include "cat.h"
#include <iostream>

Cat::Cat(const std::string& name) : Animal(name) {}

void Cat::speak() const {
    std::cout << name << " says: Meow!" << std::endl;
}
