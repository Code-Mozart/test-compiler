#pragma once

#include "Statement.h"
#include "Expression.h"

namespace AST {

	class Declaration : public Statement {
	public:
		inline virtual Type GetType() const override { return Type::Declaration; }
		inline virtual string ToString() const override {
			return Node::ToString() + "\n\t" + identifier + "\n" + value->ToString();
		}
		inline virtual string ToString(const string& tag) const {
			if (tag == "identifier") return identifier;
			else return Statement::ToString(tag);
		}
		inline virtual string TreeToString(byte indent = 0) const {
			return Node::TreeToString(indent) + "\n"
				+ GetIndentString(indent + 1) + identifier + "\n"
				+ value->TreeToString(indent + 1);
		}
	public:
		string identifier;
		ref<Expression> value;
	};

}
