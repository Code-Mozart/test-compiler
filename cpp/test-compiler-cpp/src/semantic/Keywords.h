#pragma once

#include "util/Util.h"

namespace Keywords {

	static const string ProcDecl = "proc";
	static const string VarDecl = "var";
	static const string WhileStm = "while";
	static const string IfStm = "if";
	static const string ElseStm = "else";
}

enum class Keyword {
	ProcDecl,
	VarDecl,
	WhileStm,
	IfStm,
	ElseStm,
	None
};
