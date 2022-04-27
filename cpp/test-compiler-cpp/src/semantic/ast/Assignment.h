#pragma once

#include "Statement.h"
#include "Expression.h"

namespace AST {

	class Assignment : public Statement {
	public:
		inline virtual Type GetType() const override {
			return Type::Assignment;
		}
		inline virtual string ToString(byte indent = 0) const {
			return Node::ToString(indent)
				+ GetIndentString(indent + 1) + variable + "\n"
				+ value->ToString(indent + 1);
		}
	public:
		string variable;
		ref<Expression> value = nullptr;
	};

}
