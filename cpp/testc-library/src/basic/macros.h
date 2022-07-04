#pragma once

#define GET_BIT(bits, mask) (bits & mask)
#define SET_BIT(bits, mask) bits |= mask
#define CLEAR_BIT(bits, mask) bits &= ~mask
#define FLIP_BIT(bits, mask) bits ^= mask

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)
#define ASSERT(x) if (!(x)) throw "ASSERTION FAILED AT " __FILE__ ":" STRINGIFY(__LINE__)

#define EXPAND(x) x

#define MISSING_ENUM(x) static_assert(false, x)
