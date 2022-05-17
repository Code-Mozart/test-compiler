#pragma once

#include "Node.h"
#include "Procedure.h"

namespace AST {

	class Container : public Node {
	public:
		inline virtual Type GetType() const override {
			return Type::Container;
		}
		inline virtual string ToString(byte indent = 0) const {
			string s = Node::ToString(indent);
			for (const auto& proc : procedures)
				s += proc->ToString(indent + 1);
			return s;
		}
	public:
		vector<ref<Procedure>> procedures;
		ref<SymbolTable> symbols = nullptr;
	};

}
