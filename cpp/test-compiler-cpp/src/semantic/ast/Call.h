#pragma once

#include "Statement.h"
#include "Expression.h"

namespace AST {

	class Call : public Statement {
	public:
		inline virtual Type GetType() const override { return Type::Call; }
		inline virtual string ToString() const override {
			string s;
			for (const auto& a : args) s += a->ToString();
			return Node::ToString() + "\n\t" + identifier + "()\n" + s;
		}
		inline virtual string ToString(const string& tag) const {
			if (tag == "identifier") return identifier;
			else return Statement::ToString(tag);
		}
		inline virtual string TreeToString(byte indent = 0) const {
			string s = Node::TreeToString(indent) + "\n"
				+ GetIndentString(indent + 1) + identifier + "\n";
			for (const auto& a : args)
				s += a->TreeToString(indent + 1);
			return s;
		}
	public:
		string identifier;
		vector<ref<Expression>> args;
	};

}
