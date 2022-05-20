#pragma once

#include "util/Util.h"
#include "semantic/ast/include.h"

// @improve: different maps for vars and procs so that they may have the same identifier
class SymbolTable
{
public:
	SymbolTable(const SymbolTable* pParent = nullptr) : pParent(pParent) {}
public:
	// returns nullptr or the conflicting declaration
	ref<AST::Declaration> AddVar(ref<AST::Declaration> declaration);
	
	// returns nullptr or the conflicting definition
	// resolves all pending calls waiting on this procedure
	// definition
	ref<AST::Procedure> AddProc(ref<AST::Procedure> definition);


	// returns the declaration of the variable or nullptr if the variable
	// has not been declared
	ref<AST::Declaration> GetVar(const string& identifier) const;

	// returns the definition of the procedure or nullptr if the procedure
	// was not yet been defined
	ref<AST::Procedure> GetProc(const string& identifier) const;
	
	// returns the definition of the procedure or nullptr if the procedure
	// was not yet been defined in which case the call is remembered by this
	// symbol table and gets resolved eventually when the procedure is finally
	// defined
	ref<AST::Procedure> AddCall(ref<AST::Call> call);

	inline const map<string, vector<ref<AST::Call>>>& GetUnresolvedCalls() const { return unresolvedCalls; }
private:
	map<string, vector<ref<AST::Call>>> unresolvedCalls;
	map<string, ref<AST::Procedure>> procedures;
	map<string, ref<AST::Declaration>> variables;
	const SymbolTable* pParent;
};
