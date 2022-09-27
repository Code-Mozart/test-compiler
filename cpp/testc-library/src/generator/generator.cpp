#include "generator.h"
#include "syntax/ast.h"

namespace testc {

    // STATIC PROCEDURE DECLARATIONS

    static Generator_Result generate_procedure_instructions(    Ref<AST_Procedure> proc);

    static Generator_Result generate_block_instructions(        Ref<AST_Block> block);
    static Generator_Result generate_declaration_instructions(  Ref<AST_Declaration> decl);
    static Generator_Result generate_assignment_instructions(   Ref<AST_Assignment> assignm);
    static Generator_Result generate_while_instructions(        Ref<AST_While> whil);
    static Generator_Result generate_if_else_instructions(      Ref<AST_If_Else> if_else);
    static Generator_Result generate_call_instructions(         Ref<AST_Call> call);
    
    static Generator_Result generate_constant_instructions(     Ref<AST_Constant> cnst);
    static Generator_Result generate_variable_instructions(     Ref<AST_Variable> var);
    static Generator_Result generate_binary_op_instructions(    Ref<AST_Binary_Op> bin_op);


    // STATIC PROCEDURE IMPLEMENTATIONS


    // HEADER PROCEDURE IMPLEMENTATIONS

    // Generate Instructions

    Generator_Result generate_instructions(Ref<AST_Node> node, Ref<const Symbol_Table> symbols) {
        NOT_IMPL();
    }

    Generator_Result generate_instructions(const List<Ref<AST_Procedure>>& procedures, Ref<const Symbol_Table> symbols) {
        NOT_IMPL();
    }

    // Check nothing unresolved

    Result check_nothing_unresolved(const List<Ref<Symbol>>& labels, const List<Ref<Symbol>>& dependencies) {
        NOT_IMPL();    
    }

    Result check_nothing_unresolved(const Generator_Result& generator_result) {
        NOT_IMPL();
    }

}
