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
	ref<AST::Node> BuildAST(const vector<Token>& tokens);
private:
	Keyword ParseKeyword(const string& s);
	ref<AST::Statement> ParseStatement(const vector<Token>& tokens, int& index);
	ref<AST::Expression> ParseExpression(const vector<Token>& tokens, int& index);
	ref<AST::Expression> BuildExpression(vector<const Token*>& outQueue);

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

