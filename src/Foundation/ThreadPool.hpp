#pragma once

#include <algorithm>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

class ThreadPool
{
private:

    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable cv_;
    bool stop_ = false;

public:

    ThreadPool(size_t thread_count = std::max(std::thread::hardware_concurrency(), 1u))
    {
        for (size_t i = 0; i < thread_count; ++i)
        {
            threads_.emplace_back([this]()
            {
                while (true)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex_);
                        this->cv_.wait(lock, [this]
                        {
                            // Wake up if there is a task or if we're stopping
                            return this->stop_ || !this->tasks_.empty();
                        });

                        // If we are stopping and have no tasks, break out of loop
                        if (this->stop_ && this->tasks_.empty())
                        {
                            return;
                        }

                        // Otherwise, pop a task from the queue
                        task = std::move(this->tasks_.front());
                        this->tasks_.pop();
                    }

                    // Execute the task outside the lock
                    task();
                }
            });
        }
    }

    template <typename Function, typename... Arguments>
    void Enqueue(Function&& function, Arguments&&... args)
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            tasks_.emplace(std::bind(std::forward<Function>(function), std::forward<Arguments>(args)...));
        }
        cv_.notify_one();
    }

    void Wait()
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
