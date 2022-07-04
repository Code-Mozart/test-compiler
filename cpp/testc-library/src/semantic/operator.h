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
	const string& to_string(Operator op);

}
