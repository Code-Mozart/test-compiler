#pragma once

#include "basic/include.h"

// Interface for the type checker that validates the AST (abstract syntax
// tree).
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
// -> [TYPE CHECKER]
//     v
//    [CODE GENERATOR]
//     v
//    instructions

namespace testc {

    struct Compiler_Message;
	struct AST_Node;
    struct AST_Procedure;

    // Resolves all pending usages within the given list of procedures considering
    // the given list of procedures.
    // #returns:
    //   all generated compiler messages. For now actually no compiler messages are generated.
    List<Owner<const Compiler_Message>> resolve_references(const List<Ref<AST_Procedure>>& procedures);

    // Checks the types within the given subtree and generates corresponding
    // compiler messages if language rules have been violated.
    // #parameter node:
    //   the root of the subtree that should be checked.
    // #returns:
    //   all generated compiler messages.
    List<Owner<const Compiler_Message>> type_check(Ref<const AST_Node> node);

}
