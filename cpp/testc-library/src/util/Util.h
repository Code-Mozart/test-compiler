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

// Non-owning reference (pointer) to an instance of type T.
// This is used over the raw pointer (T*) to indicate that something is just
// a reference to T and does not manage the lifetime.
// On the other hand it must be ensured that the instance of T is alive for
// the whole lifetime of this reference. This is not a runtime/compile-time
// guarantee but rather a constraint that must be met in code.
// #similar: Owner<T>
template<typename T>
using Ref = T*;

// Ownership of an instance of type T.
// The owner of an instance manages the lifetime (allocation & deallocation).
// This responsibility is indicated with this alias and therefore this
// alias is prefered over a raw pointer (T*).
// An instance should never have multiple owners otherwise bugs/problems
// might occur with managing the lifetime. If ownership is shared the usage
// of std::shared_ptr should be preferred.
// This ownership indication is no runtime/compile-time constraint but rather
// a hint to the usage of this reference.
// #similar: Ref<T>
template<typename T>
using Owner = T*;

#define GET_BIT(bits, mask) (bits & mask)
#define SET_BIT(bits, mask) bits |= mask
#define CLEAR_BIT(bits, mask) bits &= ~mask
#define FLIP_BIT(bits, mask) bits ^= mask

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)
#define ASSERT(x) if (!(x)) throw "ASSERTION FAILED AT " __FILE__ ":" STRINGIFY(__LINE__)

#define EXPAND(x) x
