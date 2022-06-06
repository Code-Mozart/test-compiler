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

template <typename T, typename... Args>
inline constexpr ref<T> RefTo(Args&&... args) { return std::make_shared<T>(std::forward<Args>(args)...); }

template <typename T, typename U>
inline constexpr ref<T> CastTo(U& u) { return std::static_pointer_cast<T>(u); }

#define GET_BIT(bits, mask) (bits & mask)
#define SET_BIT(bits, mask) bits |= mask
#define CLEAR_BIT(bits, mask) bits &= ~mask
#define FLIP_BIT(bits, mask) bits ^= mask

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)
#define ASSERT(x) if (!(x)) throw "ASSERTION FAILED AT " __FILE__ ":" STRINGIFY(__LINE__)

#define EXPAND(x) x
