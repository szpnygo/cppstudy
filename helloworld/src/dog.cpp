#include "dog.h"
#include <iostream>

Dog::Dog(const std::string& name) : Animal(name) {}

void Dog::speak() const {
    std::cout << name << " says: Meow!" << std::endl;
}
