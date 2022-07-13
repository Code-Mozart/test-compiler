#include "Parser.h"

namespace testc {

    // Procedures

    Parser_Result<AST_Procedure> parse_procedure(Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols) {
        NOT_IMPL();
    }
    
	Parser_Result_Multi<AST_Procedure> parse_procedures(Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols) {
        NOT_IMPL();
    }


    // Statements

	Parser_Result<AST_Statement> parse_statement(Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols) {
        NOT_IMPL();
    }

	Parser_Result_Multi<AST_Statement> parse_statements(Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols) {
        NOT_IMPL();
    }
    
	Parser_Result<AST_Block> parse_block(Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols) {
        NOT_IMPL();
    }

	Parser_Result<AST_Declaration> parse_declaration(Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols) {
        NOT_IMPL();
    }

	Parser_Result<AST_Assignment> parse_assignment(Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols) {
        NOT_IMPL();
    }

	Parser_Result<AST_While> parse_while(Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols) {
        NOT_IMPL();
    }

	Parser_Result<AST_If_Else> parse_if_else(Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols) {
        NOT_IMPL();
    }

	Parser_Result<AST_Call> parse_call(Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols) {
        NOT_IMPL();
    }


    // Expressions

	Parser_Result<AST_Expression> parse_expression(Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols) {
        NOT_IMPL();
    }

	Parser_Result<AST_Constant> parse_constant(Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols) {
        NOT_IMPL();
    }

	Parser_Result<AST_Variable> parse_variable(Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols) {
        NOT_IMPL();
    }
	
    Parser_Result<AST_Binary_Op> parse_binary_op(Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols) {
        NOT_IMPL();
    }

}
