#pragma once

#include "basic/include.h"

namespace testc {

	enum class Keyword {
		Variable_Decl,
		Procedure_Decl,
		While,
		If,
		Else
	};

	const string& as_string(Keyword keyword);
	inline const string& to_string(const Keyword& keyword) { return as_string(keyword); }

}
