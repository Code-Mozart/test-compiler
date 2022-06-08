#pragma once

#include "util/util.h"

namespace testc {

	enum class Operator {
		Plus,
		Minus,
		Asterisk,
		Slash,

		Less_Than
	};

	const string& as_string(Operator op);

}