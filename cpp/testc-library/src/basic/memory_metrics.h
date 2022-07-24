#pragma once

#include "basic/include.h"

namespace testc {

	struct Memory_Metrics {
		size_t system_allocations = 0;
		size_t total_system_memory = 0;
		size_t total_memory_used = 0;
		size_t total_allocations = 0;
		size_t total_deallocations = 0;
	};

	const Memory_Metrics& get_memory_metrics();

}
