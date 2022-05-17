#pragma once

#include "util/Util.h"
#include "semantic/ast/include.h"
#include "semantic/Symbol.h"

// @improve: different maps for vars and procs so that they may have the same identifier
class SymbolTable
{
public:
	SymbolTable(const SymbolTable* pParent = nullptr) : pParent(pParent) {}
public:
	inline ref<AST::Node> Find(const string& identifier) const {
		const Symbol* sym = FindSymbol(identifier);
		return sym ? sym->definition : nullptr;
	}
	inline ref<AST::Node> FindProc(const string& identifier) const { return FindType(identifier, SymbolType::Procedure); }
	inline ref<AST::Node> FindVar(const string& identifier) const { return FindType(identifier, SymbolType::Variable); }

	inline bool AddProc(const string& identifier, ref<AST::Node> definition) {
		return Add(identifier, SymbolType::Procedure, definition);
	}
	inline bool AddVar(const string& identifier, ref<AST::Node> definition) {
		return Add(identifier, SymbolType::Variable, definition);
	}
private:
	const Symbol* FindSymbol(const string& identifier) const;
	inline ref<AST::Node> FindType(const string& identifier, SymbolType type) const {
		const Symbol* sym = FindSymbol(identifier);
		return (sym && sym->type == type) ? sym->definition : nullptr;
	}
	bool Add(const string& identifier, SymbolType type, ref<AST::Node> definition);
private:
	map<string, Symbol> data;
	const SymbolTable* pParent;
};
