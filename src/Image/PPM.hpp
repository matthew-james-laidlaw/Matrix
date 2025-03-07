#pragma once

#include <Expect.hpp>
#include <Tensor.hpp>

#include <fstream>

auto EncodePpm(const std::string& filename, Tensor<uint8_t, 3> const& rgb) -> void
{
    auto [height, width, channels] = rgb.Shape();
    Expect(channels == 3, "Input tensor must have 3 channels (RGB)");

    std::ofstream outfile(filename, std::ios::binary);
    if (!outfile)
    {
        throw std::runtime_error("error: unable to open file for writing");
    }

    outfile << "P6" << std::endl;
    outfile << width << " " << height << std::endl;
    outfile << "255" << std::endl;

    outfile.write(reinterpret_cast<const char*>(rgb.Data()), rgb.Size() * sizeof(uint8_t));

    outfile.close();
}
