#pragma once

#include "Exception.h"
#include "errh/ErrorHandler.h"
#include "syntax/Token.h"
#include "semantic/ast/include.h"
#include "semantic/Keywords.h"
#include "syntax/Lexer.h"
#include "semantic/SymbolTable.h"

class Parser {
public:
	Parser(const FileInfo& srcInfo, Lexer& lexer, ErrorHandler& errh);
public:
	ref<AST::Container> BuildAST(SymbolTable* builtIns);
private:
	Keyword ParseKeyword(const string& s);
	ref<AST::Statement> ParseStatement(SymbolTable* symbols);
	ref<AST::Expression> ParseExpression(SymbolTable* symbols, short minPrec = -1);
	ref<AST::Sequence> ParseSequence(SymbolTable* symbols);
	ref<AST::Procedure> ParseProcedure(SymbolTable* symbols);

	ref<AST::Declaration> ParseDeclaration(SymbolTable* symbols);
	ref<AST::While> ParseWhile(SymbolTable* symbols);
	ref<AST::IfElse> ParseIfElse(SymbolTable* symbols);
	ref<AST::Assignment> ParseAssignment(SymbolTable* symbols, const Token* identTkn);
	ref<AST::Call> ParseCall(SymbolTable* symbols, const Token* identTkn);

	ref<AST::Expression> ParsePrimary(SymbolTable* symbols);
	ref<AST::Expression> ParseCondition(SymbolTable* symbols);
	ref<AST::Sequence> ParseBlock(SymbolTable* symbols);
	

	bool RequireToken(const Token* token, TokenType type, const string& errMsg);
	bool RequireOperator(const Token* token, AST::ASTOperator op);
	inline bool RequireIdentifier(const Token* token) {
		return RequireToken(token, TokenType::Identifier, "expected an identifier");
	}

	inline bool RequireToken(TokenType type, const string& errMsg) {
		return RequireToken(lexer.Advance(), type, errMsg);
	}
	inline bool RequireToken(TokenType type) {
		return RequireToken(lexer.Advance(), type, string() + "expected a '" + ToChar(type) + "'");
	}
	inline bool RequireOperator(AST::ASTOperator op) {
		return RequireOperator(lexer.Advance(), op);
	}
	inline bool RequireIdentifier() {
		return RequireIdentifier(lexer.Advance());
	}
	inline bool RequireSemicolon() {
		return RequireToken(lexer.Advance(), TokenType::Semicolon, "expected a ';'");
	}

	template<typename T>
	ref<T> CreateNode(const Token* tkn);

	void PushErr(const string& text, const Token* tkn);
private:
	const FileInfo& srcInfo;
	Lexer& lexer;
	ErrorHandler& errh;
};

struct ParseException : public Exception {
public:
	ParseException(const Token* t)
		: Exception("failed to parse token '" + t->ToString() + "'") {}
};

