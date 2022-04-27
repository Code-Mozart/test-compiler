#pragma once

#include "ASTOperator.h"
#include "Expression.h"

namespace AST {

	class BinaryOperator : public Expression {
	public:
		inline virtual Type GetType() const override {
			return Type::BinOp;
		}
		inline virtual string ToString(byte indent = 0) const {
			return Node::ToString(indent)
				+ GetIndentString(indent + 1) + (char)op + "\n"
				+ lhs->ToString(indent + 1) + rhs->ToString(indent + 1);
		}
	public:
		ASTOperator op;
		ref<Expression> lhs = nullptr;
		ref<Expression> rhs = nullptr;
	};

}
