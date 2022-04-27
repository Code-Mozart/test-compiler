#pragma once

#include "Statement.h"
#include "BinaryOperator.h"

namespace AST {

	class While : public Statement {
	public:
		inline virtual Type GetType() const override { return Type::While; }
		inline virtual string ToString() const override {
			string s;
			for (const auto& stm : body) s += stm->ToString();
			return Node::ToString() + "\n" + condition->ToString() + s;
		}
		inline virtual string TreeToString(byte indent = 0) const {
			string s = Node::TreeToString(indent) + "\n"
				+ condition->TreeToString(indent + 1);
			for (const auto& stm : body)
				s += stm->TreeToString(indent + 1);
			return s;
		}
	public:
		ref<BinaryOperator> condition;
		vector<ref<Statement>> body;
	};

}
