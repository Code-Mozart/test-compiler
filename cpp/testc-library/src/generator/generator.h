#pragma once

#include "basic/include.h"
#include "common/result.h"
#include "generator/instruction.h"

// Interface for the code generator which takes the previously assembled
// informations in the form of the abstract syntax tree and generates
// virtual machine instructions for it.
//
//    src-code
//     v
//    [LEXER]
//     v
//    tokens
//     v
//    [PARSER]
//     v
//    ast
//     v
//    [TYPE CHECKER]
//     v
// -> [CODE GENERATOR]
//     v
//    instructions

namespace testc {

    struct Symbol;
    struct AST_Node;
    struct AST_Procedure;

    struct Generator_Result {
		List<Owner<const Compiler_Message>> messages;

        // The generated virtual machine instructions.
        List<Owner<Instruction>> instructions;

        // A map of symbols to their respective labels.
        Map<Ref<Symbol>, Ref<Instruction>> labels;

        // A map of symbols to their depending usages.
        Map<Ref<Symbol>, List<Ref<Instruction>>> dependencies;
    };
    
	// #returns:
	//   whether generating instructions resulted in a failure or not.
	inline bool has_failed(const Generator_Result& result) { return false; }

    // Generates virtual machine instructions for the given node.
    Generator_Result generate_instructions(Ref<AST_Node> node);

    // Generates virtual machine instructions for the given procedure nodes.
    // The generated instructions of the different nodes are simply concatenated
    // in the resulting list. All dependencies between the generated instruction
    // of the nodes will be resolved.
    Generator_Result generate_instructions(const List<Ref<AST_Procedure>>& procedures);

    // Checks if there are unresolved dependencies and generates corresponding errors
    // if so.
    Result check_nothing_unresolved(const List<Ref<Symbol>>& labels, const List<Ref<Symbol>>& dependencies);

    // Checks if there are unresolved dependencies and generates corresponding errors
    // if so.
    Result check_nothing_unresolved(const Generator_Result& generator_result);

}
