#pragma once

#include "basic/include.h"
#include "common/compiler_message.h"

namespace testc {

	struct Result {
		bool success = true;
		List<Owner<const Compiler_Message>> messages;
	};

	inline bool has_failed(const Result& result) { return !result.success; }

}
