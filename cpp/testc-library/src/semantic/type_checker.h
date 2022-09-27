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

#pragma once

#include "basic/include.h"
#include "common/result.h"

namespace testc {

    struct Compiler_Message;
	struct AST_Node;
    struct AST_Procedure;
    struct Symbol_Table;

    // Resolves all pending references within the given list of procedures considering
    // the given list of procedures.
    // #returns:
    //   success if all pending references have been resolved or failure and the corresponding
    //   error messages otherwise.
    Result resolve_references(
        const List<Ref<AST_Procedure>>& procedures, Ref<Symbol_Table> symbols);

    // Checks the types within the given subtree and generates corresponding
    // compiler messages if language rules have been violated.
    // #parameter node:
    //   the root of the subtree that should be checked.
    // #returns:
    //   success if there are no type violations or failure and the corresponding error messages
    //   otherwise.
    Result type_check(Ref<const AST_Node> node);

}
