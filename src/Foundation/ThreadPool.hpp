#pragma once

#include "Number.hpp"

#include <mutex>
#include <thread>
#include <vector>

class ThreadPool
{
private:

    std::vector<std::jthread> threads_;
    std::mutex mutex_;

public:

    ThreadPool(usize num_threads)
    {
        threads_.reserve(num_threads);
        for (usize i = 0; i < num_threads; ++i)
        {
            threads_[i] = std::jthread([]()
            {

            });
        }
    }

    ~ThreadPool()
    {
        Wait();
    }

    template <typename Function>
    auto Enqueue(Function&& function) -> void
    {

    }

    auto Wait() -> void
    {
        for (auto& thread : threads_)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
    }

};
