#include "request.h"

#include <atomic>
#include <chrono> // for std::chrono
#include <cpr/cpr.h>
#include <future>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread> // for std::this_thread
#include <vector>

// std::mutex cout_mutex;       // 保护 std::cout 的锁
// std::atomic<int> counter(0); // 计数器
// const int MAX_RETRIES = 3;

// std::string get_and_process(const std::string& url) {
//     for (int i = 0; i < MAX_RETRIES; ++i) {           // 重试循环
//         std::lock_guard<std::mutex> lock(cout_mutex); // 保护计数器的锁
//         ++counter;
//         cpr::Response r =
//             cpr::Get(cpr::Url{url}, cpr::Timeout{1000}); //
//             设置超时为1000毫秒
//         if (r.status_code == 200) {
//             try {
//                 nlohmann::json j = nlohmann::json::parse(r.text);
//                 std::ostringstream oss;
//                 oss << "URL: " << url << '\n';
//                 oss << "Username: " << j["login"] << '\n';
//                 oss << "ID: " << j["id"] << '\n';
//                 return oss.str();
//             } catch (nlohmann::json::parse_error& e) { // 添加错误处理
//                 std::cout << "Parse error: " << e.what() << '\n';
//             }
//         }
//         if (r.error || r.status_code != 200) { // 错误处理
//             // 休眠一段时间后重试
//             std::this_thread::sleep_for(std::chrono::seconds(1));
//             if (r.error) {
//                 std::cout << "Error: " << r.error.message << " " << url <<
//                 '\n';
//             } else {
//                 std::cout << "Status: " << r.status_code << " " << url <<
//                 '\n';
//             }
//         }
//     }

//     return "Failed to get user info from " + url + '\n';
// }

int main() {
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
            std::async(std::launch::async, ApiRequest::request, user));
    }

    // 等待所有异步操作完成，并打印结果
    for (auto& fut : futures) {
        fut.wait();             // 等待异步操作完成
        std::cout << fut.get(); // 获取并打印结果
    }

    // std::cout << "Total requests: " << counter << '\n';

    return 0;
}