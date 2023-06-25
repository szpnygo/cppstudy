#include "thread_pool.h"

// 定义一个模拟的网络请求函数，该函数会在线程池中异步调用
std::string fetchDataFromUrl(std::string url) {
    // 模拟网络请求的耗时
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 返回获取到的数据
    return "data from " + url;
}

int main() {
    // 创建一个拥有4个线程的线程池
    ThreadPool pool(4);

    // 创建一个future的向量，用于保存每个异步操作的返回结果
    std::vector<std::future<std::string>> results;

    // 定义要爬取的网址列表
    std::vector<std::string> urls = {"url1", "url2", "url3", "url4", "url5"};

    // 为每个网址启动一个异步操作
    for (auto &url : urls) {
        results.emplace_back(pool.enqueue(1, fetchDataFromUrl, url));
    }

    // 输出每个异步操作的结果
    for (auto &&result : results) {
        std::cout << result.get() << std::endl;
    }

    return 0;
}
