#include <iostream>
#include <memory>
#include <vector>

#include "cat.h"
#include "dog.h"

template <typename T>
void makeSpeak(const T& animals) {
    for (const auto& animal : animals) {
        animal->speak();
    }
}

int main() {
    std::cout << "Hello World!" << std::endl;

    std::vector<std::unique_ptr<Animal>> animals;
    animals.push_back(std::make_unique<Dog>("Bobby"));
    animals.push_back(std::make_unique<Cat>("Kitty"));

    makeSpeak(animals);

    return 0;
}