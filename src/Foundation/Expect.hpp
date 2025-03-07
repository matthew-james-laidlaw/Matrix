#pragma once

#include <stdexcept>
#include <string>

template <typename Exception = std::runtime_error>
auto Expect(bool expected, std::string const& message = "internal error")
{
    if (!expected)
    {
        throw Exception(message);
    }
}
