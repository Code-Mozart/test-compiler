#include "SymbolTable.h"

bool SymbolTable::Add(const string& identifier, SymbolType type, ref<AST::Node> definition)
{
	if (pParent && pParent->Find(identifier))
		return false;
	auto [iter, success] = data.emplace(std::make_pair(identifier, Symbol{ identifier, type, definition }));
	return success;
}

const Symbol* SymbolTable::FindSymbol(const string& identifier) const
{
	// search in bottom-up order
	auto iter = data.find(identifier);
	if (iter != data.end()) {
		return &(iter->second);
	}
	else if (pParent) {
		return pParent->FindSymbol(identifier);
	}
	else {
		return nullptr;
	}
}
