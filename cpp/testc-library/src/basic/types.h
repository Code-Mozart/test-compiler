#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <filesystem>

namespace testc {

    using byte = unsigned char;
    using uint = unsigned int;
    using ulong = unsigned long;
    using std::string;
    using std::pair;

    // Non-owning reference (pointer) to an instance of type T.
    // This is used over the raw pointer (T*) to indicate that something is just
    // a reference to T and does not manage the lifetime.
    // On the other hand it must be ensured that the instance of T is alive for
    // the whole lifetime of this reference. This is not a runtime/compile-time
    // guarantee but rather a constraint that must be met in code.
    // #similar: Owner<T>
    // #similar: Shared<T>
    template<typename T>
    using Ref = T*;

    // Ownership of an instance of type T.
    // The owner of an instance manages the lifetime (allocation & deallocation).
    // This responsibility is indicated with this alias and therefore this
    // alias is prefered over a raw pointer (T*).
    // An instance should never have multiple owners otherwise bugs/problems
    // might occur with managing the lifetime. If ownership is shared Shared<T> should
    // be used instead.
    // This ownership indication is no runtime/compile-time constraint but rather
    // a hint to the usage of this reference.
    // #similar: Ref<T>
    // #similar: Shared<T>
    template<typename T>
    using Owner = T*;

    // Indicates a shared ownership of type T.
    // The owners must manage the lifetime (allocatation & deallocation) and ensure
    // themselfes that the instance is not deallocated multiple times nor accessed
    // after being deallocated.
    // #similar: Ref<T>
    // #similar: Owner<T>
    template<typename T>
    using Shared = T*;

    // Non-owning reference to an array of type
    template<typename T>
    using Array_Ref = T*;

    template<typename T> struct assert_ptr;
    template<typename T> struct assert_ptr<T*> {
        using type = T*;
    };

    // Use this alias over the raw pointer (T*) to indicate that T is nullable.
    template<typename T>
    using Nullable = typename assert_ptr<T>::type;

    // List of T's.
    // This list owns the elements it holds and is responsible for deallocating them.
    // It is generally not safe to take a reference/pointer to an element in this list
    // as the list might reallocate its elements when modified.
    template<typename T>
    using List = std::vector<T>;

    // Map of keys K to values V.
    // This map owns its elements and is responsible for managing their lifetime.
    // Therefore it is generally not safe to take a reference/pointer to an element in
    // in this map as is might reallocate the elements when modified.
    template<typename K, typename V>
    using Map = std::map<K, V>;

    // Set of T's.
    // This set owns its elements and is responsible for managing their lifetime.
    // Therefore it is generally not safe to take a reference/pointer to an element in
    // in this map as is might reallocate the elements when modified.
    template<typename T>
    using Set = std::unordered_set<T>;

    using File = std::filesystem::path;

    using std::to_string;

}
