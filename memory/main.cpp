#include <iostream>
#include <memory>

class Demo {
public:
    Demo() { std::cout << "Demo object is created.\n"; }
    ~Demo() { std::cout << "Demo object is destroyed.\n"; }
    void print() { std::cout << "Demo object is running.\n"; }
};

// 函数接收一个Demo原始指针
void UseRawPointer(Demo* raw) {
    std::cout << "----- Using raw pointer in function -----\n";
    raw->print();
    // 注意我们在这里不删除原始指针，因为我们并不拥有这个对象
}

// 函数接收一个unique_ptr
// 注意我们需要使用std::move来传递unique_ptr，因为unique_ptr不能被复制，只能被移动
void UseUniquePointer(std::unique_ptr<Demo> unique) {
    std::cout << "----- Using unique pointer in function -----\n";
    unique->print();
    // unique_ptr会在离开作用域时自动删除其指向的对象
}

// 函数接收一个shared_ptr
// shared_ptr可以被复制，这会增加其指向的对象的引用计数
void UseSharedPointer(std::shared_ptr<Demo> shared) {
    std::cout << "----- Using shared pointer in function -----\n";
    shared->print();
    std::cout << "Shared pointer reference count: " << shared.use_count()
              << "\n";
    // 当shared_ptr离开作用域时，引用计数减1，如果引用计数变为0，则删除其指向的对象
}

void SharedPointerSharing(std::shared_ptr<Demo> shared) {
    std::cout << "----- Shared pointer sharing demonstration -----\n";
    shared->print();
    std::cout << "Shared pointer reference count: " << shared.use_count()
              << "\n";

    // 创建一个新的shared_ptr，共享同一个对象
    std::shared_ptr<Demo> shared2 = shared;
    shared2->print();
    std::cout << "Shared pointer reference count: " << shared.use_count()
              << "\n";
    // 在函数结束后，shared2离开作用域，引用计数减1
}

// 新的函数，用于演示std::move
void MoveUniquePointer() {
    std::cout << "----- Move unique pointer demonstration -----\n";
    // 创建一个unique_ptr
    std::unique_ptr<Demo> unique1(new Demo());

    // 使用std::move将unique1的所有权移动到unique2
    std::unique_ptr<Demo> unique2 = std::move(unique1);
    // 现在，unique1不再拥有任何对象，而unique2拥有原来unique1所指向的对象

    if (unique1) {
        std::cout << "unique1 owns the object.\n";
    } else {
        std::cout << "unique1 no longer owns the object.\n";
    }

    if (unique2) {
        std::cout << "unique2 now owns the object.\n";
    } else {
        std::cout << "unique2 does not own the object.\n";
    }
    // 在函数结束后，unique2离开作用域，删除其拥有的对象
}

int main() {
    std::cout << "----- Creating pointers -----\n";
    Demo* raw = new Demo();
    std::unique_ptr<Demo> unique(new Demo());
    std::shared_ptr<Demo> shared(new Demo());

    std::cout << "Shared pointer reference count: " << shared.use_count()
              << "\n";

    UseRawPointer(raw);
    UseUniquePointer(std::move(unique));  // 使用std::move来传递unique_ptr
    UseSharedPointer(shared);
    std::cout << "Shared pointer reference count: " << shared.use_count()
              << "\n";
    SharedPointerSharing(shared);  // 这里再次使用shared_ptr，演示其共享特性
    std::cout << "Shared pointer reference count: " << shared.use_count()
              << "\n";
    // 执行新的函数
    MoveUniquePointer();

    // 需要手动删除原始指针，否则会导致内存泄漏
    delete raw;

    return 0;
}
