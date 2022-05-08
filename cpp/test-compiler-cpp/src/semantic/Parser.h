#pragma once

#include "Exception.h"
#include "errh/ErrorHandler.h"
#include "syntax/Token.h"
#include "semantic/ast/include.h"
#include "semantic/Keywords.h"
#include "syntax/Lexer.h"

class Parser {
public:
	Parser(Lexer& lexer, ErrorHandler& errh);
public:
	ref<AST::Node> BuildAST();
private:
	Keyword ParseKeyword(const string& s);
	ref<AST::Statement> ParseStatement();
	ref<AST::Expression> ParseExpression(short minPrec = -1);
	ref<AST::Sequence> ParseSequence();

	ref<AST::Declaration> ParseDeclaration();
	ref<AST::While> ParseWhile();
	ref<AST::IfElse> ParseIfElse();
	ref<AST::Assignment> ParseAssignment(const Token* identTkn);
	ref<AST::Call> ParseCall(const Token* identTkn);

	ref<AST::Expression> ParsePrimary();
	ref<AST::Expression> ParseCondition();
	ref<AST::Sequence> ParseBlock();

	// @refactor: refactor the logic into the following methods

	// ParseCondition() = '( EXPR )'
	// ParseBlock() = '{ STM... }'
	

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
	Lexer& lexer;
	ErrorHandler& errh;
};

struct ParseException : public Exception {
public:
	ParseException(const Token* t)
		: Exception("failed to parse token '" + t->ToString() + "'") {}
};

