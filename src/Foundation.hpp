#pragma once

#include <cstdint>
#include <concepts>

using i8  = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using usize = std::size_t;
using isize = std::ptrdiff_t;

template <typename T>
concept Signed = std::same_as<T, i8>
			  || std::same_as<T, i16>
			  || std::same_as<T, i32>
			  || std::same_as<T, i64>;

template <typename T>
concept Unsigned = std::same_as<T, u8>
			    || std::same_as<T, u16>
			    || std::same_as<T, u32>
			    || std::same_as<T, u64>;

template <typename T>
concept Integer = Signed<T> || Unsigned<T>;

using f32 = float;
using f64 = double;

template <typename T>
concept Real = std::same_as<T, f32>
            || std::same_as<T, f64>;

template <typename T>
concept Number = Integer<T> || Real<T>;
