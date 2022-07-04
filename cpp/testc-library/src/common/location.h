#pragma once

#include "basic/include.h"
#include "util/string_view.h"

namespace testc {

	constexpr Ref<const File> No_File = nullptr;
	constexpr ulong No_Line = 0;
	constexpr ulong No_Pos = 0;

	// Used to refer to a location in a source code.
	struct Location {
		Ref<const File> filepath = No_File;
		ulong line = No_Line;
		ulong pos = No_Pos;
	};

	string as_string(const Location& loc);
	string to_string(const Location& loc);

}
