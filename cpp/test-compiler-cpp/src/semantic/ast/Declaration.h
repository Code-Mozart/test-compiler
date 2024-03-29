#pragma once

#include "Statement.h"
#include "Expression.h"

namespace AST {

	class Declaration : public Statement {
	public:
		inline virtual Type GetType() const override {
			return Type::Declaration;
		}
		inline virtual string ToString(byte indent = 0) const {
			return Node::ToString(indent)
				+ GetIndentString(indent + 1) + "ident: " + identifier + "\n"
				+ (value ? value->ToString(indent + 1) : "");
		}
	public:
		string identifier;
		
		// nullable
		ref<Expression> value;
	};

}
