#include "Matrix.hpp"
#include "Dispatcher.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <tuple>

using namespace isp;

// Helper function for gradual color mapping using logarithmic scaling and gamma correction.
std::tuple<double, double, double> ColorMap(double normalized, double gamma = 0.8, double alpha = 10.0)
{
    // Apply logarithmic scaling.
    // This will stretch out lower values and compress higher values.
    double scaled = std::log(1.0 + alpha * normalized) / std::log(1.0 + alpha);
    
    // Then apply gamma correction.
    double corrected = std::pow(scaled, gamma);
    
    // Map the corrected value to RGB using a polynomial mapping.
    double r = 9.0 * (1 - corrected) * std::pow(corrected, 3) * 255.0;
    double g = 15.0 * std::pow(1 - corrected, 2) * std::pow(corrected, 2) * 255.0;
    double b = 8.5 * std::pow(1 - corrected, 3) * corrected * 255.0;
    
    return std::make_tuple(r, g, b);
}

// Generate a colored Mandelbrot set using double precision, smooth coloring,
// and additional color mapping/filtering, returning three planar matrices (red, green, blue).
auto GenerateColoredMandelbrot(usize height, usize width)
{
    // Use double precision for the color channels.
    Matrix<double, ThreadingPolicy::Threaded, ArithmeticPolicy::Naive> red(height, width);
    Matrix<double, ThreadingPolicy::Threaded, ArithmeticPolicy::Naive> green(height, width);
    Matrix<double, ThreadingPolicy::Threaded, ArithmeticPolicy::Naive> blue(height, width);

    const int maxIter = 1000;
    // Define the region in the complex plane.
    const double xMin = -2.0, xMax = 1.0;
    const double yMin = -1.0, yMax = 1.0;
    const double dx = (xMax - xMin) / (width - 1);
    const double dy = (yMax - yMin) / (height - 1);

    // Use the threaded dispatch to parallelize the computation.
    DispatchBlocks(height, width, [&](size_t y, size_t x)
    {
        double c_re = xMin + x * dx;
        double c_im = yMin + y * dy;
        double z_re = 0.0, z_im = 0.0;
        int iter = 0;
        while ((z_re * z_re + z_im * z_im) <= 4.0 && iter < maxIter)
        {
            double temp = z_re * z_re - z_im * z_im + c_re;
            z_im = 2 * z_re * z_im + c_im;
            z_re = temp;
            ++iter;
        }

        // Compute a smooth iteration count.
        double smoothIter = static_cast<double>(iter);
        if (iter < maxIter)
        {
            double mag = std::sqrt(z_re * z_re + z_im * z_im);
            // Avoid log(0) issues.
            if (mag > 0.0)
            {
                smoothIter = iter + 1 - std::log(std::log(mag)) / std::log(2.0);
            }
        }

        // Normalize the smooth iteration count to [0, 1].
        double normalized = smoothIter / maxIter;
        if (iter == maxIter) 
        {
            normalized = 0.0; // Points inside the set are colored black.
        }

        // Apply our color mapping/filtering function.
        auto [r, g, b] = ColorMap(normalized);

        red(y, x) = r;
        green(y, x) = g;
        blue(y, x) = b;
    });

    return std::make_tuple(red, green, blue);
}

// Write three planar matrices as a PPM (P3) file with three color channels.
// The matrices contain double values that are cast to integers in the 0–255 range.
template <typename MatrixType>
void WritePpm(const MatrixType& red,
              const MatrixType& green,
              const MatrixType& blue,
              const std::string& filename)
{
    // Ensure that the dimensions of the three matrices match.
    if (red.Height() != green.Height() || red.Height() != blue.Height() ||
        red.Width() != green.Width() || red.Width() != blue.Width())
    {
        throw std::runtime_error("Channel matrices dimensions do not match.");
    }

    std::ofstream ofs(filename);
    if (!ofs)
    {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    usize height = red.Height();
    usize width = red.Width();

    // Write the PPM header: "P3" format, image dimensions, and maximum color value.
    ofs << "P3\n" << width << " " << height << "\n255\n";

    // Write pixel data, casting double values to int.
    for (usize y = 0; y < height; ++y)
    {
        for (usize x = 0; x < width; ++x)
        {
            ofs << static_cast<int>(red(y, x)) << " "
                << static_cast<int>(green(y, x)) << " "
                << static_cast<int>(blue(y, x)) << " ";
        }
        ofs << "\n";
    }
    ofs.close();
}

// Example main function to generate and write a smooth-colored Mandelbrot PPM image.
int main()
{
    const usize height = 5000;
    const usize width = 5000;
    const std::string filename = "mandelbrot.ppm";

    try
    {
        // Generate colored Mandelbrot image using double precision, smooth coloring,
        // and added color mapping/filtering.
        auto [red, green, blue] = GenerateColoredMandelbrot(height, width);
        WritePpm(red, green, blue, filename);
        std::cout << "Smooth colored Mandelbrot PPM image written to " << filename << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
