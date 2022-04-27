#pragma once

#include "Node.h"

namespace AST {

	class BuiltIn : public Node {
	public:
		inline virtual Type GetType() const override { return Type::BuiltIn; }
		inline virtual string ToString() const override {
			return AST::ToString(GetType());
		}
		inline virtual string TreeToString(byte indent = 0) const {
			return Node::ToString() + "\n";
		}
	};

}
