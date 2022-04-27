#pragma once

#include "Node.h"
#include "Statement.h"

namespace AST {

	class Sequence : public Node {
	public:
		inline virtual Type GetType() const override { return Type::Sequence; }
		inline virtual string ToString() const override {
			string s;
			for (const auto& stm : statements) s += stm->ToString();
			return Node::ToString() + "\n" + s;
		}
		inline virtual string TreeToString(byte indent = 0) const {
			string s = Node::TreeToString(indent) + "\n";
			for (const auto& stm : statements)
				s += stm->TreeToString(indent + 1);
			return s;
		}
	public:
		vector<ref<Statement>> statements;
	};

}
