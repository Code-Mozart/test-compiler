#pragma once

#include "basic/types.h"
#include "basic/macros.h"
#include "basic/exception.h"

namespace testc {

	// Allocates memory for the new instance and constructs it in-place.
	// #parameter args:
	//   The constructor arguments. Those will be perfectly forwarded into the constructor.
	template<typename T, typename... Args>
	T* construct(Args&&... args);

	// Destructs T and deallocates it's memory.
	template<typename T>
	void destruct(T* instance);

}
