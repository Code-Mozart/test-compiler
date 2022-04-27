#pragma once

#include "Expression.h"

namespace AST {

	class Constant : public Expression {
	public:
		inline virtual Type GetType() const override { return Type::Const; }
		inline virtual string ToString() const override {
			return Node::ToString() + "\n\t" + std::to_string(value) + "\n";
		}
		inline virtual string TreeToString(byte indent = 0) const {
			return Node::TreeToString(indent) + "\n"
				+ GetIndentString(indent + 1) + std::to_string(value) + "\n";
		}
	public:
		short value;
	};

}
