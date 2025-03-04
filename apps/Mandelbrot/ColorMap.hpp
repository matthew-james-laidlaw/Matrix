#pragma once

#include <array>

struct Color
{
    double r, g, b;
};

using Palette = std::array<Color, 16>;

static const Palette plasma16 = {{
    {0.050383, 0.029803, 0.527975},
    {0.220057, 0.031733, 0.577770},
    {0.390935, 0.120072, 0.642666},
    {0.556753, 0.209570, 0.706718},
    {0.691500, 0.301554, 0.753605},
    {0.800805, 0.395432, 0.787964},
    {0.894855, 0.488268, 0.800058},
    {0.974083, 0.576579, 0.792126},
    {0.989363, 0.665161, 0.749019},
    {0.980466, 0.748046, 0.685294},
    {0.957676, 0.821573, 0.611141},
    {0.915517, 0.886242, 0.549019},
    {0.855929, 0.937742, 0.505085},
    {0.779101, 0.975572, 0.488296},
    {0.681585, 0.993476, 0.553897},
    {0.489055, 0.998535, 0.686159},
}};

static const Palette inferno16 = {{
    {0.001462, 0.000466, 0.013866},
    {0.071176, 0.016284, 0.097467},
    {0.180653, 0.045510, 0.216018},
    {0.282885, 0.095953, 0.341051},
    {0.392636, 0.149897, 0.459686},
    {0.507860, 0.201772, 0.563265},
    {0.620300, 0.249552, 0.645943},
    {0.730889, 0.291330, 0.705497},
    {0.837015, 0.330279, 0.750423},
    {0.932119, 0.376821, 0.783976},
    {0.993248, 0.432737, 0.800192},
    {0.996096, 0.525576, 0.753632},
    {0.983450, 0.622158, 0.629723},
    {0.963911, 0.717061, 0.477504},
    {0.941029, 0.815517, 0.271305},
    {0.987533, 0.991438, 0.749504},
}};

static const Palette magma16 = {{
    {0.001462, 0.000466, 0.013866},
    {0.018426, 0.014381, 0.084426},
    {0.047190, 0.033945, 0.149914},
    {0.084103, 0.052407, 0.193828},
    {0.144870, 0.083565, 0.244538},
    {0.229904, 0.131360, 0.297620},
    {0.297237, 0.167644, 0.328039},
    {0.374835, 0.205128, 0.354373},
    {0.507988, 0.261720, 0.391039},
    {0.657103, 0.318700, 0.419197},
    {0.789441, 0.366240, 0.435302},
    {0.898078, 0.404282, 0.443285},
    {0.952971, 0.423286, 0.445606},
    {0.989953, 0.456938, 0.451345},
    {0.920997, 0.508797, 0.456701},
    {0.987053, 0.991438, 0.749504},
}};

Color interpolateColor(Palette const& palette, double t)
{
    t = std::max(std::min(t, 1.0), 0.0);

    // Scale t to [0, palette.size()-1].
    long double pos = t * (palette.size() - 1);
    size_t index = static_cast<size_t>(pos);
    long double frac = pos - index;

    if (index >= palette.size() - 1)
    {
        // t is at or beyond the last bin
        return palette.back();
    }

    // Linear interpolate between palette[index] and palette[index+1].
    const Color &c0 = palette[index];
    const Color &c1 = palette[index + 1];
    Color result;
    result.r = c0.r + frac * (c1.r - c0.r);
    result.g = c0.g + frac * (c1.g - c0.g);
    result.b = c0.b + frac * (c1.b - c0.b);
    return result;
}

//--------------------------------------
// 3) An enum + function to pick which palette to use
//--------------------------------------
enum class Colormap
{
    Plasma,
    Inferno,
    Magma
};

Color getColorFromColormap(Colormap cm, long double t)
{
    switch (cm)
    {
    case Colormap::Plasma:
        return interpolateColor(plasma16, t);
    case Colormap::Inferno:
        return interpolateColor(inferno16, t);
    case Colormap::Magma:
        return interpolateColor(magma16, t);
    }
    // Default fallback (shouldn't happen)
    return interpolateColor(plasma16, t);
}
