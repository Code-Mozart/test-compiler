#pragma once

#include "basic/include.h"

// Interface for the parser that analyzes a token stream and builds
// an abstract syntax tree out of them.
//
//    src-code
//     v
//    [LEXER]
//     v
//    tokens
//     v
// -> [PARSER]
//     v
//    ast
//     v
//    [TYPE CHECKER]
//     v
//    [CODE GENERATOR]
//     v
//    instructions

namespace testc {

	struct Compiler_Message;
	struct Lexer;
	struct Symbol_Table;
	struct AST_Procedure;
	struct AST_Statement;
	struct AST_Block;
	struct AST_Declaration;
	struct AST_Assignment;
	struct AST_While;
	struct AST_If_Else;
	struct AST_Call;
	struct AST_Expression;
	struct AST_Constant;
	struct AST_Variable;
	struct AST_Binary_Op;

	template<typename T>
	struct Parser_Result {
		List<Owner<const Compiler_Message>> messages;
		Nullable<Owner<T>> node;
	};

	// #returns:
	//   whether parsing the token stream to an ast node resulted in a failure or not.
	template<typename T>
	inline bool has_failed(const Parser_Result<T>& result) { return result.node == nullptr; }

	template<typename T>
	struct Parser_Result_Multi {
		List<Owner<const Compiler_Message>> messages;
		List<Owner<T>> nodes;
	};

	// #returns:
	//   whether parsing the token stream to procedure nodes resulted in a failure or not.
	template<typename T>
	inline bool has_failed(const Parser_Result_Multi<T>& result) { return result.nodes.empty(); }

	// use the following procedures to parse the corresponding node from the
	// token stream
	// #parameter lexer:
	//   A lexer acting as an interface to the token stream.
	//   Only consumes the tokens it needs to parse the node.
	// #returns:
	//   the node or nullptr and an error when such a node could not be constructed
	//   from the given token stream.
	
	Parser_Result<AST_Procedure>		parse_procedure(	Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols);
	Parser_Result_Multi<AST_Procedure>	parse_procedures(	Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols);

	// Tries to parse a single statement from the token stream. Use this procedure
	// if any statement is allowed. To parse only a particular statement (and fail
	// when this expected statement can not be parsed from the token stream) use
	// the respective procedures (parse_if, parse_while, parse_declaration, ...)
	Parser_Result<AST_Statement>		parse_statement(	Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols);
	
	// Tries to parse multiple statements from the token stream. Stops if the next tokens
	// can not be parsed into another statement in which case the lexer points to the first token
	// that has not yet been parsed (i.e. the token that terminated this sequence of statements).
	Parser_Result_Multi<AST_Statement>	parse_statements(	Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols);

	Parser_Result<AST_Block>			parse_block(		Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols);
	Parser_Result<AST_Declaration>		parse_declaration(	Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols);
	Parser_Result<AST_Assignment>		parse_assignment(	Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols);
	Parser_Result<AST_While>			parse_while(		Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols);
	Parser_Result<AST_If_Else>			parse_if_else(		Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols);
	Parser_Result<AST_Call>				parse_call(			Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols);

	// Tries to parse an expression from the token stream. Use this procedure if any
	// expression is allowed. To parse only a particular expression type (and fail when
	// this excepted expression type can not be parsed from the token stream) use the
	// respective procedure (parse_constant, parse_binary_op, ...)
	Parser_Result<AST_Expression>		parse_expression(	Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols);

	Parser_Result<AST_Constant>			parse_constant(		Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols);
	Parser_Result<AST_Variable>			parse_variable(		Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols);
	Parser_Result<AST_Binary_Op>		parse_binary_op(	Ref<Lexer> lexer, Nullable<Ref<Symbol_Table>> symbols);

}
