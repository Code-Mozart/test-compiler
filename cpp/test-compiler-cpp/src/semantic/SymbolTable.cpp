#include "SymbolTable.h"

bool SymbolTable::Add(const string& identifier, ref<AST::Node> definition)
{
	if (pParent && pParent->Find(identifier))
		return false;
	auto [iter, success] = data.emplace(std::make_pair(identifier, Symbol{ identifier, definition }));
	return success;
}

ref<AST::Node> SymbolTable::Find(const string& identifier)
{
	// search in bottom-up order
	auto iter = data.find(identifier);
	if (iter != data.end()) {
		return iter->second.definition;
	}
	else if (pParent) {
		return pParent->Find(identifier);
	}
	else {
		return nullptr;
	}
}
