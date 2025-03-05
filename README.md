# Matrix
[![unit](https://github.com/matthew-james-laidlaw/Matrix/actions/workflows/unit.yml/badge.svg?branch=main)](https://github.com/matthew-james-laidlaw/Matrix/actions/workflows/unit.yml)

A (mostly completely self-implemented) library providing a tensor class supporting up to 3 dimensions. Focused on image processing. Libraries pulled in for argument parsing, testing, and image encoding e.g. lodepng).

# Configure
```
cmake --list-presets
Available configure presets:

  "arm64-linux"
  "arm64-windows"
  "x64-linux"
  "x64-windows"
```

# Build

# Run

# Test

# Design Decisions
* The focus of this library is on image processing, thus the tensor class is not fully generic for any order. Really, this library supports 1D, 2D, and 3D tensors where a 3D tensor can represent a planar image (channels x height x width) or an interleaved image (height x width x channels).
* The class is generic where possible, and when it is not, template specializations define how operations are defined for that order. One example is indexing. Generically finding a linear index into a 1D, 2D, or 3D tensor would be more computationally intensive than just applying the arithmetic needed to find a linear index for the given order. Thus, a template specialization exists for each case.
* The build system is extreme overkill for the scope of this application, however I had the desire to explore what a modern, somewhat comprehensive, cmake build system might look like, and how it would connect with CI/CD (how it would make my life easier or harder).
    * Learnings: This cmake setup may work well for cross compilation but I felt like I had this explosion of permutations of OS/architecture, but in the end it wasn't much different than just doing none of this and letting CMake detect the architecture and OS for me. At least now I have specific places where I can target toolchain changes if that were necessary. Say I needed to define a flag only for windows builds, I could do that via this setup.

# Resources
* [CMake Project Structure Convention](https://cliutils.gitlab.io/modern-cmake/chapters/basics/structure.html)
