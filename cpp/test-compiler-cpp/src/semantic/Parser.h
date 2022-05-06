#pragma once

#include "Exception.h"
#include "errh/ErrorHandler.h"
#include "syntax/Token.h"
#include "semantic/ast/include.h"
#include "semantic/Keywords.h"

class Parser {
public:
	Parser(ErrorHandler& errh) : errh(errh) {}
public:
	// @robustness: replace tokens ref with lexer.peek_next()
	ref<AST::Node> BuildAST(const vector<Token>& tokens);
private:
	Keyword ParseKeyword(const string& s);
	ref<AST::Statement> ParseStatement(const vector<Token>& tokens, int& index);
	ref<AST::Expression> ParseExpression(const vector<Token>& tokens, int& index, short minPrec = -1);
	ref<AST::Sequence> ParseSequence(const vector<Token>& tokens, int& index);

	ref<AST::Declaration> ParseDeclaration(const vector<Token>& tokens, int& index);
	ref<AST::While> ParseWhile(const vector<Token>& tokens, int& index);
	ref<AST::IfElse> ParseIfElse(const vector<Token>& tokens, int& index);
	ref<AST::Assignment> ParseAssignment(const vector<Token>& tokens, int& index, const Token& identTkn);
	ref<AST::Call> ParseCall(const vector<Token>& tokens, int& index, const Token& identTkn);

	ref<AST::Expression> ParsePrimary(const vector<Token>& tokens, int& index);
	ref<AST::Expression> ParseCondition(const vector<Token>& tokens, int& index);
	ref<AST::Sequence> ParseBlock(const vector<Token>& tokens, int& index);

	// @refactor: refactor the logic into the following methods

	// ParseCondition() = '( EXPR )'
	// ParseBlock() = '{ STM... }'
	

	bool RequireToken(const Token& token, TokenType type, const string& errMsg);
	bool RequireOperator(const Token& token, AST::ASTOperator op);
	inline bool RequireIdentifier(const Token& token) {
		return RequireToken(token, TokenType::Identifier, "expected an identifier");
	}

	inline bool RequireToken(const vector<Token>& tokens, int& index, TokenType type, const string& errMsg) {
		return RequireToken(tokens[index++], type, errMsg);
	}
	inline bool RequireToken(const vector<Token>& tokens, int& index, TokenType type) {
		return RequireToken(tokens[index++], type, string() + "expected a '" + ToChar(type) + "'");
	}
	inline bool RequireOperator(const vector<Token>& tokens, int& index, AST::ASTOperator op) {
		return RequireOperator(tokens[index++], op);
	}
	inline bool RequireIdentifier(const vector<Token>& tokens, int& index) {
		return RequireIdentifier(tokens[index++]);
	}
	inline bool RequireSemicolon(const vector<Token>& tokens, int& index) {
		return RequireToken(tokens[index++], TokenType::Semicolon, "expected a ';'");
	}

	template<typename T>
	ref<T> CreateNode(const Token& tkn);

	void PushErr(const string& text, const Token& tkn);
private:
	ErrorHandler& errh;
};

struct ParseException : public Exception {
public:
	ParseException(const Token& t)
		: Exception("failed to parse token '" + t.ToString() + "'") {}
};

