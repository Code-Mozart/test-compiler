#pragma once

#include "Statement.h"
#include "Expression.h"

namespace AST {

	class Assignment : public Statement {
	public:
		inline virtual Type GetType() const override { return Type::Assignment; }
		inline virtual string ToString() const override {
			return Node::ToString() + "\n\t" + variable + "\n" + value->ToString();
		}
		inline virtual string ToString(const string& tag) const {
			if (tag == "identifier") return variable;
			else return Statement::ToString(tag);
		}
		inline virtual string TreeToString(byte indent = 0) const {
			return Node::TreeToString(indent) + "\n"
				+ GetIndentString(indent + 1) + variable + "\n"
				+ value->TreeToString(indent + 1);
		}
	public:
		string variable;
		ref<Expression> value = nullptr;
	};

}
