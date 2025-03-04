#pragma once

#include <Matrix.hpp>
#include <Number.hpp>
#include <Expect.hpp>

#include <fstream>
#include <vector>

auto Interleave(Matrix<uint8_t> const& red, Matrix<uint8_t> const& green, Matrix<uint8_t> const& blue) -> std::vector<uint8_t>
{
    Expect(DimensionsEqual(red, green, blue));

    size_t height = red.Height();
    size_t width = red.Width();

    size_t pixel_stride = 3;
    size_t row_stride = width * pixel_stride;

    size_t size = height * row_stride;
    
    std::vector<uint8_t> interleaved(size);
    
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            size_t interleaved_index = (y * row_stride) + (x * pixel_stride);
            interleaved[interleaved_index + 0] = red(y, x);
            interleaved[interleaved_index + 1] = green(y, x);
            interleaved[interleaved_index + 2] = blue(y, x);
        }
    }
    
    return interleaved;
}

auto EncodePpm(const std::string &filename, Matrix<uint8_t> const& red, Matrix<uint8_t> const& green, Matrix<uint8_t> const& blue) -> void
{
    Expect(DimensionsEqual(red, green, blue));

    size_t height = red.Height();
    size_t width = red.Width();

    std::ofstream outfile(filename, std::ios::binary);
    Expect(static_cast<bool>(outfile));

    outfile << "P6" << std::endl;
    outfile << width << " " << height << std::endl;
    outfile << "255" << std::endl;

    auto interleaved = Interleave(red, green, blue);

    outfile.write(reinterpret_cast<const char*>(interleaved.data()), interleaved.size() * sizeof(uint8_t));
    
    outfile.close();
}
