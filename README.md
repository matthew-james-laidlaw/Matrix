# Matrix
[![unit](https://github.com/matthew-james-laidlaw/Matrix/actions/workflows/unit.yml/badge.svg?branch=main)](https://github.com/matthew-james-laidlaw/Matrix/actions/workflows/unit.yml)

A library providing a tensor class supporting up to 3 dimensions. Focused on image processing.

# Design Decisions
* The focus of this library is on image processing, thus the tensor class is not fully generic for any order. Really, this library supports 1D, 2D, and 3D tensors where a 3D tensor can represent a planar image (channels x height x width) or an interleaved image (height x width x channels).
* The class is generic where possible, and when it is not, template specializations define how operations are defined for that order. One example is indexing. Generically finding a linear index into a 1D, 2D, or 3D tensor would be more computationally intensive than just applying the arithmetic needed to find a linear index for the given order. Thus, a template specialization exists for each case.

# Resources
* [CMake Project Structure Convention](https://cliutils.gitlab.io/modern-cmake/chapters/basics/structure.html)
