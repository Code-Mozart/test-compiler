#pragma once

#include <stdexcept>
#include "util/Util.h"
#include "Exception.h"
#include "semantic/ast/include.h"

class RuntimeStack {
public:
	void PutVar(const string& identifier);
	inline void Grow(byte amount = 1) { size += amount; }
	inline void Shrink(byte amount = 1) {
		if ((size -= amount) < 0)
			throw IncorrectImplException(__FILE__, __LINE__, "predicted stack size < 0");
	}

	inline void EnterScope(ref<AST::Node> scope) { scopes.emplace_back(scope); }
	byte ExitScope();

	inline ulong GetOffset(const string& identifier) const {
		try {
			return size - locations.at(identifier).second;
		}
		catch (const std::out_of_range& e) {
			throw IncorrectImplException(__FILE__, __LINE__, "predicted stack didnt contain var '" + identifier + "'");
		}
	}
private:
	ulong size = 0;
	vector<ref<AST::Node>> scopes;
	map<string, pair<ref<AST::Node>, ulong>> locations;
};
