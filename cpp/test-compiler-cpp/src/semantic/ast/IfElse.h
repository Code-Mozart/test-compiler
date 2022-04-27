#pragma once

#include "Statement.h"
#include "BinaryOperator.h"

namespace AST {

	class IfElse : public Statement {
	public:
		inline virtual Type GetType() const override {
			return Type::IfElse;
		}
		inline virtual string ToString(byte indent = 0) const {
			string s = Node::ToString(indent)
				+ condition->ToString(indent + 1);
			s += GetIndentString(indent + 1) + "IF BODY\n";
			for (const auto& stm : ifBody)
				s += stm->ToString(indent + 2);
			s += GetIndentString(indent + 1) + "ELSE BODY\n";
			for (const auto& stm : elseBody)
				s += stm->ToString(indent + 2);
			return s;
		}
	public:
		ref<Expression> condition;
		vector<ref<Statement>> ifBody;
		vector<ref<Statement>> elseBody;
	};

}
