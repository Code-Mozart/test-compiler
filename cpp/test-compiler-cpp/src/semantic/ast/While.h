#pragma once

#include "Statement.h"
#include "BinaryOperator.h"

namespace AST {

	class While : public Statement {
	public:
		inline virtual Type GetType() const override {
			return Type::While;
		}
		inline virtual string ToString(byte indent = 0) const {
			string s = Node::ToString(indent)
				+ condition->ToString(indent + 1);
			s += body->ToString(indent + 1);
			return s;
		}
	public:
		ref<Expression> condition;
		ref<Sequence> body;
	};

}
