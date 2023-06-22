// 引入必要的头文件
#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>

// 定义线程池类
class ThreadPool {
public:
    // 构造函数，传入线程数目
    ThreadPool(size_t threads);

    // 添加任务到任务队列，使用模板函数支持多种函数签名
    template <class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result_t<F, Args...>>;

    // 析构函数，会通知所有线程停止，并等待它们结束
    ~ThreadPool();

private:
    // 工作线程
    std::vector<std::thread> workers;

    // 任务队列，用std::function包装各种函数
    std::queue<std::function<void()>> tasks;

    // 互斥锁，保护任务队列
    std::mutex queue_mutex;

    // 条件变量，用于线程之间的同步
    std::condition_variable condition;

    // 原子标志，表示线程池是否停止
    std::atomic<bool> stop;

    // 工作线程的主循环
    void worker();
};

// 构造函数实现
ThreadPool::ThreadPool(size_t threads) : stop(false) {
    // 创建指定数量的工作线程，每个线程都运行worker()函数
    for(size_t i = 0; i < threads; ++i) {
        workers.emplace_back(&ThreadPool::worker, this);
    }
}

// 工作线程的主循环
void ThreadPool::worker() {
    // 不断地从任务队列中取出任务并执行，直到线程池停止
    while(!stop) {
        // 函数对象，用于保存从任务队列中取出的任务
        std::function<void()> task;

        {
            // 创建独占锁，保护任务队列
            std::unique_lock<std::mutex> lock(this->queue_mutex);

            // 等待条件变量被通知，或者任务队列非空，或者线程池停止
            this->condition.wait(lock, [this]{ return this->stop || !this->tasks.empty(); });

            // 如果线程池停止并且任务队列为空，那么结束线程
            if(this->stop && this->tasks.empty()) return;

            // 取出任务队列中的一个任务
            task = std::move(this->tasks.front());

            // 将已经取出的任务从任务队列中移除
            this->tasks.pop();
        }

        // 执行任务
        task();
    }
}

// 添加任务到任务队列
template <class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::invoke_result_t<F, Args...>> {
    // 通过模板参数推导任务的返回类型
    using return_type = typename std::invoke_result_t<F, Args...>;

    // 创建一个std::packaged_task，它包装了我们的任务
    auto task = std::make_shared<std::packaged_task<return_type()> >(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    // 获取std::packaged_task的std::future，以便我们可以等待任务完成并获取结果
    std::future<return_type> res = task->get_future();

    {
        // 创建独占锁，保护任务队列
        std::unique_lock<std::mutex> lock(queue_mutex);

        // 如果线程池已经停止，那么无法添加新的任务
        if(stop) throw std::runtime_error("enqueue on stopped ThreadPool");

        // 将任务添加到任务队列中
        tasks.emplace([task](){ (*task)(); });
    }

    // 通知一个正在等待的工作线程
    condition.notify_one();

    // 返回std::future，通过它可以获取任务的结果
    return res;
}

// 析构函数
ThreadPool::~ThreadPool() {
    // 设置线程池的停止标志
    stop = true;

    // 唤醒所有等待的线程
    condition.notify_all();

    // 等待所有工作线程结束
    for(std::thread &worker: workers) worker.join();
}

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
    for(auto &url : urls) {
        results.emplace_back(
            pool.enqueue(fetchDataFromUrl, url)
        );
    }

    // 输出每个异步操作的结果
    for(auto &&result : results) {
        std::cout << result.get() << std::endl;
    }

    return 0;
}
