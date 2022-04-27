#pragma once

#include "Node.h"

namespace AST {

	class Expression : public Node {
	public:
		inline virtual Type GetType() const override {
			return Type::Expression;
		}
	};

}
