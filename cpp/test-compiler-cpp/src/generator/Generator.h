#pragma once

#include "util/Util.h"
#include "semantic/ast/include.h"
#include "generator/RuntimeStack.h"
#include "generator/Instruction.h"
#include "generator/PlaceholderHandler.h"

class CodeGenerator {
public:
	string Generate(ref<AST::Container> root);
private:
	vector<Instruction>& Generate(ref<AST::Node> node, vector<Instruction>& instructions, RuntimeStack& stack, PlaceholderHandler& phh);
};
