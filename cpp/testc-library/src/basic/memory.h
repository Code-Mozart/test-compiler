#pragma once

#include "basic/types.h"
#include "basic/macros.h"
#include "basic/exception.h"

namespace testc {

	// Allocates memory.
	// #parameter bytes:
	//   How many bytes of memory should be allocated.
	Owner<void> allocate(size_t bytes);

	// Releases the memory occupied by 'ptr.
	void deallocate(Owner<void> ptr);


	// Allocates as much memory as is needed for an instance of T.
	template<typename T>
	Owner<void> allocate() {
		return testc::allocate(sizeof(T));
	}

	// Allocates memory for the new instance and constructs it in-place.
	// #parameter args:
	//   The constructor arguments. Those will be perfectly forwarded into the constructor.
	template<typename T, typename... Args>
	Owner<T> construct(Args&&... args) {
        return new (testc::allocate<T>()) T(std::forward<Args>(args)...);
    }

	// Destructs T and deallocates it's memory.
	template<typename T>
	void destruct(Owner<T> instance) {
		instance->~T();
		testc::deallocate(instance);
	}

}
