#pragma once

#include "Expression.h"

namespace AST {

	class Constant : public Expression {
	public:
		inline virtual Type GetType() const override {
			return Type::Const;
		}
		inline virtual string ToString(byte indent = 0) const {
			return Node::ToString(indent)
				+ GetIndentString(indent + 1) + "val: " + std::to_string(value) + "\n";
		}
	public:
		short value;
	};

}
