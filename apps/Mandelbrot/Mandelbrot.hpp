#pragma once

#include <Tensor.hpp>
#include <NetPbm.hpp>
#include <Dispatcher.hpp>

#include "ColorMap.hpp"

#include <complex>

Tensor<uint8_t, 3> generateColoredMandelbrot(size_t width, size_t height, Colormap colormapChoice, size_t maxIterations = 1000)
{
    Tensor<uint8_t, 3> rgb(height, width, 3);

    // Region of the complex plane
    const double realMin = -2.5, realMax = 1.0;
    const double imagMin = -1.0, imagMax = 1.0;

    // Gamma correction
    const double gamma = 0.5;

    DispatchBlocks(height, width, [&](size_t y, size_t x)
    {
        // Map pixel (x,y) to the complex plane
        double real = realMin + (static_cast<double>(x) / (width - 1)) * (realMax - realMin);
        double imag = imagMin + (static_cast<double>(y) / (height - 1)) * (imagMax - imagMin);

        std::complex<double> c(real, imag);
        std::complex<double> z(0.0, 0.0);

        size_t iteration = 0;
        while (std::abs(z) < 2.0 && iteration < maxIterations) {
            z = z * z + c;
            ++iteration;
        }

        Color col;

        if (iteration < maxIterations) {
            // -- Outside the set: use colormap --
            double absz = std::abs(z);
            if (absz == 0.0) absz = 1e-10;
        
            double mu = iteration - std::log(std::log(absz)) / std::log(2.0);
            double t = mu / (double)maxIterations;
        
            // Apply gamma, clamp
            t = std::pow(t, gamma);
            t = std::clamp(t, 0.0, 1.0);
        
            // Lookup final color from chosen colormap
            col = getColorFromColormap(colormapChoice, t);
        }
        else {
            // -- Inside the set: make it pure black --
            col.r = 0.0;
            col.g = 0.0;
            col.b = 0.0;
        }

        // Convert [0..1] -> [0..255]
        rgb({y, x, 0}) = static_cast<unsigned char>(std::clamp(col.r * 255.0, 0.0, 255.0));
        rgb({y, x, 1}) = static_cast<unsigned char>(std::clamp(col.g * 255.0, 0.0, 255.0));
        rgb({y, x, 2}) = static_cast<unsigned char>(std::clamp(col.b * 255.0, 0.0, 255.0));
    });

    return rgb;
}
