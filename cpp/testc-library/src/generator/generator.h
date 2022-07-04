#pragma once

#include "basic/include.h"
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
        // The generated virtual machine instructions.
        List<Owner<Instruction>> instructions;

        // A map of symbols to their respective labels.
        Map<Ref<Symbol>, Ref<Instruction>> labels;

        // A map of symbols to their depending usages.
        Map<Ref<Symbol>, List<Ref<Instruction>>> dependencies;
    };

    // Generates virtual machine instructions for the given node.
    Generator_Result generate_instructions(Ref<AST_Node> node);

    // Generates virtual machine instructions for the given nodes.
    // The generated instructions of the different nodes are simply concatenated
    // in the resulting list. All dependencies between the generated instruction
    // of the nodes will be resolved.
    Generator_Result generate_instructions(List<Ref<AST_Node>> nodes);

}
