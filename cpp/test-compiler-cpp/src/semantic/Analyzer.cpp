#include "Analyzer.h"

#include "Exception.h"

using namespace AST;

// @refactor: clean up whitespaces, overhaul in general, resolve the @'s

bool Analyzer::ValidateAST(ref<Container> cont) {
	if (!cont) throw NullptrException("ValidateAST() called with null");
	ASSERT(cont->symbols);
	if (!cont->symbols->GetProc("main")) {
		errh.PushErr("there was no main procedure", *cont);
		return false;
	}
	return true;
}

bool Analyzer::Validate(ref<Node> node, SymbolTable* pSymbols) {
	if (!node) throw NullptrException("ValidateAST() called with null");
	return true;
}
