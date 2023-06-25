#include "thread_pool.h"

// 构造函数
ThreadPool::ThreadPool(size_t threads) : stop(false) {
    for (size_t i = 0; i < threads; ++i) {
        // 创建工作线程
        workers.emplace_back([this] { this->worker(); });
    }
}

// 工作线程的主循环
void ThreadPool::worker() {
    while (true) {
        TaskType task;
        {
            // 创建独占锁，保护任务队列
            std::unique_lock<std::mutex> lock(this->queue_mutex);

            // 等待新的任务或线程池停止
            this->condition.wait(
                lock, [this] { return this->stop || !this->tasks.empty(); });

            if (this->stop && this->tasks.empty()) {
                // 如果线程池已经停止，并且所有任务都已完成，那么结束主循环
                return;
            }

            // 从任务队列中取出一个任务
            task = std::move(this->tasks.top());
            this->tasks.pop();
        }

        // 执行任务，并处理可能抛出的异常
        try {
            task.second();
        } catch (const std::exception& e) {
            std::cerr << "Exception in ThreadPool worker: " << e.what() << '\n';
        }
    }
}

// 析构函数
ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }

    condition.notify_all();

    for (std::thread& worker : workers) {
        worker.join();
    }

    // 等待所有任务完成
    for (auto& f : futures) {
        f.wait();
    }
}
