#pragma once

#include <Number.hpp>
#include <ThreadPool.hpp>

template <typename Callable>
auto DispatchBlocks(size_t height, size_t width, Callable&& callable) -> void
{
    size_t block_height = 256;
    size_t block_width = 256;

    size_t num_blocks_y = (height + block_height - 1) / block_height;
    size_t num_blocks_x = (width  + block_width  - 1) / block_width;

    ThreadPool thread_pool;

    for (size_t block_y = 0; block_y < num_blocks_y; ++block_y)
    {
        for (size_t block_x = 0; block_x < num_blocks_x; ++block_x)
        {
            thread_pool.Enqueue([=, &callable]()
            {
                size_t y_start = block_y * block_height;
                size_t y_stop  = std::min(y_start + block_height, height);

                size_t x_start = block_x * block_width;
                size_t x_stop  = std::min(x_start + block_width, width);

                for (size_t y = y_start; y < y_stop; ++y)
                {
                    for (size_t x = x_start; x < x_stop; ++x)
                    {
                        callable(y, x);
                    }
                }
            });
        }
    }

    thread_pool.Wait();
}
