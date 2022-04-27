#pragma once

#include "Node.h"

namespace AST {

	class Statement : public Node {
	public:
		inline virtual Type GetType() const override {
			return Type::Statement;
		}
	};

}
