#include <iostream>
#include <variant>
#include <optional>
#include <type_traits>
#include <string>
#include <memory>
#include <functional>

// 定义一个结构体Person
struct Person {
    std::string name;
    int age;
    std::string country;

    // 成员函数，返回人的信息
    std::string info() const {
        return name + " is " + std::to_string(age) + " years old.";
    }

    // 成员函数，打印问候语
    void greet() {
        std::cout << "Hello, my name is " << name << ".\n";
    }
};

// 类型萃取，检测是否有info函数
template <typename, typename = std::void_t<>>
struct has_info : std::false_type {};

template <typename T>
struct has_info<T, std::void_t<decltype(&T::info)>> : std::true_type {};

// SFINAE，如果有info函数，使用此版本
template <typename T>
auto print(const T& t) -> std::enable_if_t<has_info<T>::value> {
    std::cout << t.info() << '\n';
}

// SFINAE，如果是数值类型，使用此版本
template <typename T>
auto print(const T& t) -> std::enable_if_t<std::is_arithmetic_v<T>> {
    std::cout << t << '\n';
}

// 函数，接受Person引用，并使其变老
void make_older(Person& person, int years) {
    person.age += years; // 修改引用实际上修改的是原对象
}

int main() {
    // 使用结构化绑定
    Person person {"Tom", 20, "USA"};
    auto [name, age, country] = person;
    // 打印解构后的变量
    std::cout << "Name: " << name << ", Age: " << age << ", Country: " << country << std::endl;

    // if constexpr用法
    if constexpr (sizeof(int) == 4) {
        std::cout << "This system uses 4-byte integers.\n";
    } else {
        std::cout << "This system does not use 4-byte integers.\n";
    }

    // std::optional
    std::optional<int> maybe_value;
    // 检查optional是否有值
    if (maybe_value) { 
        std::cout << "Value: " << *maybe_value << std::endl;
    } else {
        std::cout << "No value\n";
    }

    // std::variant
    std::variant<int, float, std::string> my_variant = "Hello World";
    // 获取variant中的字符串值
    std::cout << std::get<std::string>(my_variant) << std::endl; 

    // 左值、右值、左值引用、右值引用
    int a = 42; // a是左值
    int &&b = 42; // 42是右值，b是右值引用
    int &c = a; // c是对a的左值引用

    // 使用SFINAE和类型萃取
    int i = 42;
    double d = 3.14;
    print(i); // 输出：42
    print(d); // 输出：3.14
    print(person); // 输出：Tom is 20 years old.

    // std::bind用法
    auto greet_func = std::bind(&Person::greet, &person); // 绑定成员函数和对象
    greet_func(); // 调用绑定函数

    // std::forward用法
    std::string str = "Hello";
    std::cout << std::forward<std::string>(str) << std::endl; // 使用std::forward进行完美转发

    // std::make_shared用法
    auto shared_person = std::make_shared<Person>(Person{"Alice", 25, "Canada"}); // 使用智能指针std::make_shared创建对象

    return 0;
}
