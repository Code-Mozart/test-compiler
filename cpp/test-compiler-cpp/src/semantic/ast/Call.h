#pragma once

#include "Statement.h"
#include "Expression.h"

namespace AST {

	class Call : public Statement {
	public:
		inline virtual Type GetType() const override {
			return Type::Call;
		}
		inline virtual string ToString(byte indent = 0) const {
			string s = Node::ToString(indent)
				+ GetIndentString(indent + 1) + identifier + "\n";
			for (const auto& a : args)
				s += a->ToString(indent + 1);
			return s;
		}
	public:
		string identifier;
		// @later: introduce an ArgList node
		vector<ref<Expression>> args;
	};

}
