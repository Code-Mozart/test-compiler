#pragma once

#include "basic/include.h"

namespace testc {

	enum class Operator {
		Plus,
		Minus,
		Asterisk,
		Slash,

		Less_Than
	};

	const string& as_string(Operator op);
	inline const string& to_string(Operator op) { return as_string(op); }

}
