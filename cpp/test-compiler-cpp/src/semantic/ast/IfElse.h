#pragma once

#include "Statement.h"
#include "BinaryOperator.h"

namespace AST {

	class IfElse : public Statement {
	public:
		inline virtual Type GetType() const override { return Type::IfElse; }
		inline virtual string ToString() const override {
			// @missing: print out else body
			string s;
			for (const auto& stm : ifBody) s += stm->ToString();
			return Node::ToString() + "\n" + condition->ToString() + s;
		}
		inline virtual string TreeToString(byte indent = 0) const {
			string s = Node::TreeToString(indent) + "\n"
				+ condition->TreeToString(indent + 1);
			s += GetIndentString(indent + 1) + "IF BODY\n";
			for (const auto& stm : ifBody)
				s += stm->TreeToString(indent + 2);
			s += GetIndentString(indent + 1) + "ELSE BODY\n";
			for (const auto& stm : elseBody)
				s += stm->TreeToString(indent + 2);
			return s;
		}
	public:
		ref<BinaryOperator> condition;
		vector<ref<Statement>> ifBody;
		vector<ref<Statement>> elseBody;
	};

}
