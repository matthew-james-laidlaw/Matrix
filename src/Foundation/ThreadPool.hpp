#pragma once

#include "Number.hpp"

#include <algorithm>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <utility>

class ThreadPool
{
private:

    std::vector<std::jthread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable cv_;
    bool stop_ = false;

public:

    ThreadPool(usize thread_count = std::max(std::thread::hardware_concurrency(), 1u))
    {
        for (usize i = 0; i < thread_count; ++i)
        {
            threads_.emplace_back([this](std::stop_token stoken)
            {
                while (true)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex_);

                        this->cv_.wait(lock, [this, &stoken]
                        {
                            return this->stop_ || !this->tasks_.empty() || stoken.stop_requested();
                        });

                        if ((this->stop_ || stoken.stop_requested()) && this->tasks_.empty())
                        {
                            return;
                        }

                        task = std::move(this->tasks_.front());
                        this->tasks_.pop();
                    }

                    task();
                }
            });
        }
    }

    template <typename Function, typename... Arguments>
    auto Enqueue(Function&& function, Arguments&&... args) -> void
    {
        auto task = std::bind(std::forward<Function>(function), std::forward<Arguments>(args)...);
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            tasks_.emplace(std::move(task));
        }
        cv_.notify_one();
    }

    auto Wait() -> void
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            stop_ = true;
        }

        cv_.notify_all();

        for (auto& thread : threads_)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
    }

    ~ThreadPool()
    {
        Wait();
    }

};
