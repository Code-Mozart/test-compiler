#pragma once

#include "util/Util.h"

namespace AST {

	enum class Type {
		None		= 0x0,
		Sequence	= 0x10 << 0,
		BuiltIn		= 0x10 << 1,

		Statement	= 0x10 << 2,
		Declaration	= Statement | 1,
		Assignment	= Statement | 2,
		While		= Statement | 3,
		IfElse		= Statement | 4,
		Call		= Statement | 5,

		Expression	= 0x10 << 3,
		Const		= Expression | 1,
		BinOp		= Expression | 2,
		Var			= Expression | 3,
		
		Procedure	= 0x10 << 4,

		Container	= 0x10 << 5
	};

	string ToString(Type t);

}
