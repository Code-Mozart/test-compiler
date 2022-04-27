#pragma once

#include "ASTOperator.h"
#include "Expression.h"

namespace AST {

	class BinaryOperator : public Expression {
	public:
		inline virtual Type GetType() const override { return Type::BinOp; }
		inline virtual string ToString() const override {
			return Node::ToString() + "\n\t" + (char)op
				+ "\n" + lhs->ToString() + rhs->ToString();
		}
		inline virtual string TreeToString(byte indent = 0) const {
			return Node::TreeToString(indent) + "\n"
				+ GetIndentString(indent + 1) + (char)op + "\n"
				+ lhs->TreeToString(indent + 1) + rhs->TreeToString(indent + 1);
		}
	public:
		ASTOperator op;
		ref<Expression> lhs = nullptr;
		ref<Expression> rhs = nullptr;
	};

}
