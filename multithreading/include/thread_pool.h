#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

// 任务类型
using TaskType = std::pair<int, std::function<void()>>;

// 任务比较函数，优先级更高的任务会被优先执行
struct TaskCompare {
    bool operator()(const TaskType& t1, const TaskType& t2) {
        return t1.first < t2.first;
    }
};

class ThreadPool {
public:
    // 构造函数
    ThreadPool(size_t);

    // 添加任务到任务队列
    template <class F, class... Args>
    auto enqueue(int priority, F&& f, Args&&... args)
        -> std::future<typename std::invoke_result_t<F, Args...>> {
        using return_type = typename std::invoke_result_t<F, Args...>;

        // create task
        auto task_fn =
            std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::move(task_fn));

        std::future<return_type> res = task->get_future();

        {
            std::unique_lock<std::mutex> lock(queue_mutex);

            // don't allow enqueueing after stopping the pool
            if (stop) {
                throw std::runtime_error("enqueue on stopped ThreadPool");
            }

            tasks.emplace(priority, [task]() { (*task)(); });
        }

        condition.notify_one();

        return res;
    }

    // 析构函数
    ~ThreadPool();

private:
    // 工作线程的主循环
    void worker();

    // 工作线程的集合
    std::vector<std::thread> workers;

    // 任务队列
    std::priority_queue<TaskType, std::vector<TaskType>, TaskCompare> tasks;

    // 存放每一个任务的future
    std::vector<std::future<void>> futures;

    // 保护任务队列的互斥锁
    std::mutex queue_mutex;

    // 条件变量，用于同步工作线程的行为
    std::condition_variable condition;

    // 标记线程池是否已经停止
    bool stop;
};
