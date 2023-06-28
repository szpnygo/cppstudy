#include "request.h"

#include <future>
#include <iostream>
#include <thread> // for std::this_thread
#include <vector>

int main() {
    ApiRequest api_request;

    // 用户列表
    std::vector<std::string> users = {"https://api.github.com/users/github",
                                      "https://api.github.com/users/google",
                                      "https://api.github.com/users/facebook",
                                      "https://api.github.com/users/szpnygo"};
    // 存储 future 的向量
    std::vector<std::future<std::string>> futures;

    // 创建并运行线程
    for (const auto& user : users) {
        // 使用 std::async 创建异步操作，并将返回的 future 添加到向量
        futures.emplace_back(
            std::async(std::launch::async, [&api_request, user] {
                return api_request.request(user);
            }));
    }

    // 等待所有异步操作完成，并打印结果
    for (auto& fut : futures) {
        fut.wait();             // 等待异步操作完成
        std::cout << fut.get(); // 获取并打印结果
    }

    std::cout << "Total requests: " << api_request.get_counter() << '\n';

    return 0;
}