#pragma once

#include "util/Util.h"
#include "semantic/ast/include.h"

enum class SymbolType {
	Procedure, Variable
};

struct Symbol {
	string identifier;
	SymbolType type;
	ref<AST::Node> definition;
};
