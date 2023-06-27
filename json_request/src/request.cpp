#include "request.h"

#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>

const int MAX_RETRIES = 3;
std::mutex cout_mutex;       // 保护 std::cout 的锁
std::atomic<int> counter(0); // 计数器

std::string ApiRequest::request(const std::string& url) {
    std::cout << "Requesting " << url << '\n';

    for (int i = 0; i < MAX_RETRIES; ++i) {           // 重试循环
        std::lock_guard<std::mutex> lock(cout_mutex); // 保护计数器的锁
        ++counter;
        cpr::Response r =
            cpr::Get(cpr::Url{url}, cpr::Timeout{1000}); // 设置超时为1000毫秒
        if (r.status_code == 200) {
            try {
                nlohmann::json j = nlohmann::json::parse(r.text);
                std::ostringstream oss;
                oss << "URL: " << url << '\n';
                oss << "Username: " << j["login"] << '\n';
                oss << "ID: " << j["id"] << '\n';
                return oss.str();
            } catch (nlohmann::json::parse_error& e) { // 添加错误处理
                std::cout << "Parse error: " << e.what() << '\n';
            }
        }
        if (r.error || r.status_code != 200) { // 错误处理
            // 休眠一段时间后重试
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if (r.error) {
                std::cout << "Error: " << r.error.message << " " << url << '\n';
            } else {
                std::cout << "Status: " << r.status_code << " " << url << '\n';
            }
        }
    }

    return "Failed to get user info from " + url + '\n';
}