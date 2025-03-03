#pragma once

#include <Number.hpp>
#include <ThreadPool.hpp>

template <typename Callable>
auto DispatchBlocks(usize height, usize width, Callable&& callable) -> void
{
    usize block_height = 512;
    usize block_width = 512;

    usize num_blocks_y = (height + block_height - 1) / block_height;
    usize num_blocks_x = (width  + block_width  - 1) / block_width;

    ThreadPool thread_pool;

    for (usize block_y = 0; block_y < num_blocks_y; ++block_y)
    {
        for (usize block_x = 0; block_x < num_blocks_x; ++block_x)
        {
            thread_pool.Enqueue([=, &callable]()
            {
                usize y_start = block_y * block_height;
                usize y_stop  = std::min(y_start + block_height, height);

                usize x_start = block_x * block_width;
                usize x_stop  = std::min(x_start + block_width, width);

                for (usize y = y_start; y < y_stop; ++y)
                {
                    for (usize x = x_start; x < x_stop; ++x)
                    {
                        callable(y, x);
                    }
                }
            });
        }
    }

    thread_pool.Wait();
}
