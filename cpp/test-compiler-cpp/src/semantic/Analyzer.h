#pragma once

#include "errh/ErrorHandler.h"
#include "semantic/ast/include.h"
#include "semantic/SymbolTable.h"

class Analyzer {
public:
	Analyzer(ErrorHandler& errh) : errh(errh) {}
	bool ValidateAST(ref<AST::Container> cont);
private:
	bool Validate(ref<AST::Node> node, SymbolTable* pSymbols = nullptr);
private:
	ErrorHandler& errh;
};
