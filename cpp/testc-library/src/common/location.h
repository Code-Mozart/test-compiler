#pragma once

#include "util/util.h"
#include "util/string_view.h"

namespace testc {

	constexpr const String_View& No_File = String_View::Empty;
	constexpr ulong No_Line = 0;
	constexpr ulong No_Pos = 0;

	// Used to refer to a location in a source code.
	struct Location {
		String_View filepath = No_File;
		ulong line = No_Line;
		ulong pos = No_Pos;
	};

}
