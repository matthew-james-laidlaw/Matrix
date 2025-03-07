#pragma once

#include <cstdint>
#include <concepts>

template <typename T>
concept Signed = std::same_as<T, int8_t>
			  || std::same_as<T, int16_t>
			  || std::same_as<T, int32_t>
			  || std::same_as<T, int64_t>;

template <typename T>
concept Unsigned = std::same_as<T, uint8_t>
			    || std::same_as<T, uint16_t>
			    || std::same_as<T, uint32_t>
			    || std::same_as<T, uint64_t>;

template <typename T>
concept Integer = Signed<T> || Unsigned<T>;

template <typename T>
concept Real = std::same_as<T, float>
            || std::same_as<T, double>
			|| std::same_as<T, long double>;

template <typename T>
concept Number = Integer<T> || Real<T>;
