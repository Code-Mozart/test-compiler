#pragma once

#include "Node.h"
#include "Statement.h"

namespace AST {

	// @overhaul: replace with Block node inheriting from statement
	class Sequence : public Node {
	public:
		inline virtual Type GetType() const override {
			return Type::Sequence;
		}
		inline virtual string ToString(byte indent = 0) const {
			string s = Node::ToString(indent);
			for (const auto& stm : statements)
				s += stm->ToString(indent + 1);
			return s;
		}
	public:
		vector<ref<Statement>> statements;
	};

}
