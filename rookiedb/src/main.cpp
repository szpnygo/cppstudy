#include "vector_database.h"

#include <iostream>
#include <vector>

int main() {
    std::cout << "Hello World!" << std::endl;

    VectorDatabase db("test", 5, 1000, 16, 200, false);

    std::cout << db.name() << std::endl;

    std::vector<float> v = {1, 2, 3, 4, 5};
    db.add(2, v);

    // auto result = db.get(2);
    // for (const auto& val : result) {
    //     std::cout << val << std::endl;
    // }

    std::cout << db.exists(2) << std::endl;
    std::cout << db.exists(3) << std::endl;

    return 0;
}