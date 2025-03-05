#pragma once

#include <Tensor.hpp>
#include <Number.hpp>

#include <fstream>
#include <stdexcept>

template <typename Exception = std::runtime_error>
auto Expect(bool expected, std::string const& message = "internal error")
{
    if (!expected)
    {
        throw Exception(message);
    }
}

auto EncodePpm(const std::string &filename, Tensor<uint8_t, 3> rgb) -> void
{
    size_t height = rgb.Shape()[0];
    size_t width = rgb.Shape()[1];

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
