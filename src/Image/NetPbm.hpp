#pragma once

#include <Matrix.hpp>
#include <Number.hpp>
#include <Expect.hpp>

#include <fstream>
#include <vector>

bool DimensionsEqual(Matrix<u8> const& red, Matrix<u8> const& green, Matrix<u8> const& blue)
{
    return red.Height() == green.Height() && red.Height() == blue.Height()
        && red.Width()  == green.Width()  && red.Width()  == blue.Width();
}

auto Interleave(Matrix<u8> const& red, Matrix<u8> const& green, Matrix<u8> const& blue) -> std::vector<u8>
{
    Expect(DimensionsEqual(red, green, blue));

    usize height = red.Height();
    usize width = red.Width();

    usize pixel_stride = 3;
    usize row_stride = width * pixel_stride;

    usize size = height * row_stride;
    
    std::vector<u8> interleaved(size);
    
    for (usize y = 0; y < height; y++)
    {
        for (usize x = 0; x < width; x++)
        {
            usize plane_index = (y * width) + x;
            usize interleaved_index = (y * row_stride) + (x * pixel_stride);
            interleaved[interleaved_index + 0] = red[plane_index];
            interleaved[interleaved_index + 1] = green[plane_index];
            interleaved[interleaved_index + 2] = blue[plane_index];
        }
    }
    
    return interleaved;
}

auto EncodePgm(Matrix<u8> const& gray, const std::string &filename) -> void
{
    std::ofstream outfile(filename, std::ios::binary);
    Expect(outfile);
    
    outfile << "P5" << std::endl;
    outfile << gray.Width() << " " << gray.Height() << std::endl;
    outfile << "255" << std::endl;
    
    outfile.write(reinterpret_cast<const char*>(gray.Data()), gray.Size() * sizeof(u8));

    outfile.close();
}

auto EncodePpm(Matrix<u8> const& red, Matrix<u8> const& green, Matrix<u8> const& blue, const std::string &filename) -> void
{
    Expect(DimensionsEqual(red, green, blue));

    usize height = red.Height();
    usize width = red.Width();

    std::ofstream outfile(filename, std::ios::binary);
    Expect(outfile);

    outfile << "P6" << std::endl;
    outfile << width << " " << height << std::endl;
    outfile << "255" << std::endl;

    auto interleaved = Interleave(red, green, blue);

    outfile.write(reinterpret_cast<const char*>(interleaved.data()), interleaved.size() * sizeof(u8));
    
    outfile.close();
}
