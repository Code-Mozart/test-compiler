#include "SymbolTable.h"

SymbolTable::SymbolTable(SymbolTable* pParent)
	: pParent(pParent)
{
	if (pParent) {
		pParent->children.emplace_back(this);
	}
}

ref<AST::Declaration> SymbolTable::AddVar(ref<AST::Declaration> decl) {
	if (pParent) {
		auto conflictingDecl = pParent->GetVar(decl->identifier);
		if (conflictingDecl) return conflictingDecl;
	}
	auto [iter, success] = variables.emplace(std::make_pair(decl->identifier, decl));
	if (success) {
		return nullptr;
	}
	else {
		return iter->second;
	}
}

ref<AST::Procedure> SymbolTable::AddProc(ref<AST::Procedure> def) {
	if (pParent) {
		auto conflictingDef = pParent->GetProc(def->identifier);
		if (conflictingDef) return conflictingDef;
	}
	auto [procIter, success] = procedures.emplace(std::make_pair(def->identifier, def));
	if (success) {
		Resolve(def);
		return nullptr;
	}
	else {
		return procIter->second;
	}
}

ref<AST::Declaration> SymbolTable::GetVar(const string& identifier) const {
	// search in bottom-up order
	auto iter = variables.find(identifier);
	if (iter != variables.end()) {
		return iter->second;
	}
	else if (pParent) {
		return pParent->GetVar(identifier);
	}
	else {
		return nullptr;
	}
}

ref<AST::Procedure> SymbolTable::GetProc(const string& identifier) const {
	// search in bottom-up order
	auto iter = procedures.find(identifier);
	if (iter != procedures.end()) {
		return iter->second;
	}
	else if (pParent) {
		return pParent->GetProc(identifier);
	}
	else {
		return nullptr;
	}
}

ref<AST::Procedure> SymbolTable::AddCall(ref<AST::Call> call) {
	auto def = GetProc(call->identifier);
	if (def) {
		return def;
	}
	else {
		unresolvedCalls[call->identifier].emplace_back(call);
		return nullptr;
	}
}

void SymbolTable::Resolve(ref<AST::Procedure> def) {
	auto unresolvedIter = unresolvedCalls.find(def->identifier);
	if (unresolvedIter != unresolvedCalls.end()) {
		// could resolve all calls here if they would require a reference to the def
		unresolvedCalls.erase(unresolvedIter);
	}
	for (auto child : children) {
		child->Resolve(def);
	}
}
