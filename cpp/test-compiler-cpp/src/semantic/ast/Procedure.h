#pragma once

#include "Node.h"
#include "Sequence.h"

namespace AST {

	class Procedure : public Node {
	public:
		inline virtual Type GetType() const override {
			return Type::Procedure;
		}
		inline virtual string ToString(byte indent = 0) const {
			return Node::ToString(indent)
				+ GetIndentString(indent + 1) + "ident: " + identifier + "\n"
				+ body->ToString(indent + 1);
		}
	public:
		string identifier;
		ref<Sequence> body;
	};

}
