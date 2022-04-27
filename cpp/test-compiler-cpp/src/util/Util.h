#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>

using byte = unsigned char;
using uint = unsigned int;
using ulong = unsigned long;
using std::string;
using std::vector;
using std::map;
using std::pair;

template<typename T>
using ref = std::shared_ptr<T>;

template <typename T>
inline constexpr static ref<T> RefTo() { return std::make_shared<T>(); }

#define GET_BIT(bits, mask) (bits & mask)
#define SET_BIT(bits, mask) bits |= mask
#define CLEAR_BIT(bits, mask) bits &= ~mask
#define FLIP_BIT(bits, mask) bits ^= mask
