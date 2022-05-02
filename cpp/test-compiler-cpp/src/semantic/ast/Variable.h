#pragma once

#include "Expression.h"

namespace AST {

	class Variable : public Expression {
	public:
		inline virtual Type GetType() const override {
			return Type::Var;
		}
		inline virtual string ToString(byte indent = 0) const {
			return Node::ToString(indent) + GetIndentString(indent + 1)
				+ "ident: " + identifier + "\n";
		}
	public:
		string identifier;
	};

}
