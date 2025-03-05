#include "argparse/argparse.hpp"

#include "Mandelbrot.hpp"

#include <Tensor.hpp>
#include <NetPbm.hpp>

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("mandelbrot_generator_colormap");

    // Positional argument for output file
    program.add_argument("output")
        .help("Path for the .ppm file to be saved");

    // Optional argument for colormap
    program.add_argument("--colormap")
        .default_value(std::string("plasma"))
        .help("Which colormap to use: plasma, inferno, or magma");

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error &err)
    {
        std::cerr << err.what() << "\n";
        std::cerr << program;
        return 1;
    }

    // Get user choices from CLI
    std::string outputPath = program.get<std::string>("output");
    std::string colormapStr = program.get<std::string>("--colormap");

    // Map string -> enum
    Colormap colormapChoice;
    if (colormapStr == "plasma")
    {
        colormapChoice = Colormap::Plasma;
    }
    else if (colormapStr == "inferno")
    {
        colormapChoice = Colormap::Inferno;
    }
    else if (colormapStr == "magma")
    {
        colormapChoice = Colormap::Magma;
    }
    else
    {
        std::cerr << "Unknown colormap \"" << colormapStr << "\", defaulting to plasma.\n";
        colormapChoice = Colormap::Plasma;
    }

    // Render parameters
    const size_t width = 3840; // 4K resolution
    const size_t height = 2160;
    const size_t maxIterations = 200;

    // Generate the fractal
    auto rgb = generateColoredMandelbrot(width, height, colormapChoice, maxIterations);

    // Write out the PPM
    EncodePpm(outputPath, rgb);

    return 0;
}
