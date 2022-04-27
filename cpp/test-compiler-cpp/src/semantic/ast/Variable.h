#pragma once

#include "Expression.h"

namespace AST {

	class Variable : public Expression {
	public:
		inline virtual Type GetType() const override { return Type::Var; }
		inline virtual string ToString() const override {
			return Node::ToString() + "\n\t" + identifier + "\n";
		}
		inline virtual string ToString(const string& tag) const {
			if (tag == "identifier") return identifier;
			else return Expression::ToString(tag);
		}
		inline virtual string TreeToString(byte indent = 0) const {
			return Node::TreeToString(indent) + "\n"
				+ GetIndentString(indent + 1) + identifier + "\n";
		}
	public:
		string identifier;
	};

}
