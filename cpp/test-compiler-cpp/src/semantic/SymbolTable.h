#pragma once

#include "util/Util.h"
#include "semantic/ast/include.h"

struct Symbol {
	string identifier;
	// @placeholder: type information
	ref<AST::Node> definition;
};

class SymbolTable
{
public:
	SymbolTable(SymbolTable* pParent = nullptr) : pParent(pParent) {}
public:
	bool Add(const string& identifier, ref<AST::Node> definition);
	ref<AST::Node> Find(const string& identifier);
private:
	map<string, Symbol> data;
	SymbolTable* pParent;
};
