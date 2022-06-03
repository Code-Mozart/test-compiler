#pragma once

#include "Node.h"
#include "Sequence.h"
#include "Variable.h"

namespace AST {

	class Procedure : public Node {
	public:
		inline virtual Type GetType() const override {
			return Type::Procedure;
		}
		inline virtual string ToString(byte indent = 0) const {
			string s;
			for (const auto& param : parameters)
				s += param->ToString(indent + 1);
			return Node::ToString(indent)
				+ GetIndentString(indent + 1) + "ident: " + identifier + "\n"
				+ s
				+ body->ToString(indent + 1);
		}
	public:
		string identifier;
		ref<Sequence> body;
		vector<ref<Declaration>> parameters;
	};

}
