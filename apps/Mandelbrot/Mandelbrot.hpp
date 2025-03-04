#pragma once

#include <Matrix.hpp>
#include <NetPbm.hpp>
#include <Dispatcher.hpp>

#include "ColorMap.hpp"

#include <complex>

void generateColoredMandelbrot(Matrix<uint8_t> &red,
                               Matrix<uint8_t> &green,
                               Matrix<uint8_t> &blue,
                               size_t width,
                               size_t height,
                               Colormap colormapChoice,
                               size_t maxIterations = 1000)
{
    // Region of the complex plane
    const double realMin = -2.5, realMax = 1.0;
    const double imagMin = -1.0, imagMax = 1.0;

    // Gamma correction
    const double gamma = 0.5L;

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
        
            double mu = iteration - std::logl(std::logl(absz)) / std::logl(2.0);
            double t = mu / (double)maxIterations;
        
            // Apply gamma, clamp
            t = std::powl(t, gamma);
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
        red(y, x)   = static_cast<unsigned char>(std::clamp(col.r * 255.0, 0.0, 255.0));
        green(y, x) = static_cast<unsigned char>(std::clamp(col.g * 255.0, 0.0, 255.0));
        blue(y, x)  = static_cast<unsigned char>(std::clamp(col.b * 255.0, 0.0, 255.0)); });
}
