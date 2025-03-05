#pragma once

#include <Expect.hpp>
#include <Number.hpp>
#include <Tensor.hpp>
#include <lodepng.h>

#include <fstream>
#include <stdexcept>

auto EncodePng(const std::string& filename, Tensor<uint8_t, 3> const& rgb) -> void
{
    auto [height, width, channels] = rgb.Shape();
    Expect(channels == 3, "Input tensor must have 3 channels (RGB)");

    // Directly pass the raw data pointer to LodePNG (no copy)
    unsigned error = lodepng::encode(filename, rgb.Data(), width, height, LCT_RGB, 8);
    if (error)
    {
        throw std::runtime_error("LodePNG encoding error: " + std::string(lodepng_error_text(error)));
    }
}
