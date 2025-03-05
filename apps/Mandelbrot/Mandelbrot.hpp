#pragma once

#include <Tensor.hpp>
#include <Dispatcher.hpp>

#include "ColorMap.hpp"

#include <complex>

auto GenerateMandelbrot(size_t height, size_t width) -> Tensor<float, 2>
{
    size_t maxIterations = 200;

    Tensor<float, 2> intensity({height, width}, 0.0f);

    const double realMin = -2.5, realMax = 1.0;
    const double imagMin = -1.0, imagMax = 1.0;

    Dispatch2d(height, width, [&](size_t y, size_t x)
    {
        double real = realMin + (static_cast<double>(x) / (width - 1)) * (realMax - realMin);
        double imag = imagMin + (static_cast<double>(y) / (height - 1)) * (imagMax - imagMin);

        std::complex<double> c(real, imag);
        std::complex<double> z(0.0, 0.0);

        size_t iteration = 0;
        while (std::abs(z) < 2.0 && iteration < maxIterations) {
            z = z * z + c;
            ++iteration;
        }

        if (iteration < maxIterations) {
            double absz = std::abs(z);
            if (absz == 0.0) absz = 1e-10;
            double mu = iteration - std::log(std::log(absz)) / std::log(2.0);
            intensity({y, x}) = mu / static_cast<double>(maxIterations);
        }
        else {
            intensity({y, x}) = 0.0;
        }
    });

    return intensity;
}

auto GenerateMandelbrotImage(size_t height, size_t width, Colormap colormap) -> Tensor<uint8_t, 3>
{
    auto intensity = GenerateMandelbrot(height, width);
    Tensor<uint8_t, 3> rgb({height, width, 3}, 0);

    const double gamma = 1.0;

    Dispatch2d(height, width, [&](size_t y, size_t x)
    {
        double t = intensity({y, x});
        t = std::pow(t, gamma);
        t = std::clamp(t, 0.0, 1.0);

        Color col = getColorFromColormap(colormap, t);

        rgb({y, x, 0}) = static_cast<unsigned char>(std::clamp(col.r * 255.0, 0.0, 255.0));
        rgb({y, x, 1}) = static_cast<unsigned char>(std::clamp(col.g * 255.0, 0.0, 255.0));
        rgb({y, x, 2}) = static_cast<unsigned char>(std::clamp(col.b * 255.0, 0.0, 255.0));
    });

    return rgb;
}
