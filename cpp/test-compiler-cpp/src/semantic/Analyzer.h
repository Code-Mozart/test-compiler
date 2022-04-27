#pragma once

#include "ErrorHandler.h"
#include "semantic/ast/include.h"
#include "semantic/SymbolTable.h"

class Analyzer {
public:
	Analyzer(ErrorHandler& errh) : errh(errh) {}
	bool ValidateAST(ref<AST::Node> node, SymbolTable* pSymbols = nullptr);
private:
	ErrorHandler& errh;
};
