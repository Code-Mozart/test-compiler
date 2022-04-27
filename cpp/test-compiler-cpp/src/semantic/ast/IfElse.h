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
			// ifBody must never be null
			s += ifBody->ToString(indent + 1);
			if (elseBody)
				s += elseBody->ToString(indent + 1);
			return s;
		}
	public:
		ref<Expression> condition;
		ref<Sequence> ifBody;
		ref<Sequence> elseBody;
	};

}
