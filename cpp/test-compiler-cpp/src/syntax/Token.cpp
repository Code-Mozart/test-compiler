#include "Token.h"

#include "Exception.h"

string ToString(TokenType type) {
	switch (type) {
	case TokenType::Identifier:		return "Identifier";
	case TokenType::Literal:		return "Literal";
	case TokenType::LParen:			return "LParen";
	case TokenType::RParen:			return "RParen";
	case TokenType::Operator:		return "Operator";
	case TokenType::LBrace:			return "LBrace";
	case TokenType::RBrace:			return "RBrace";
	case TokenType::Semicolon:		return "Semicolon";
	case TokenType::Comma:			return "Comma";
	}
	throw IncorrectImplException(__FILE__, __LINE__, "enum case not handled");
}

char ToChar(TokenType type) {
	switch (type) {
	case TokenType::LParen:			return '(';
	case TokenType::RParen:			return ')';
	case TokenType::LBrace:			return '{';
	case TokenType::RBrace:			return '}';
	case TokenType::Semicolon:		return ';';
	case TokenType::Comma:			return ',';
	}
	throw IncorrectImplException(__FILE__, __LINE__, "enum case not handled");
}

string Token::ToString() const {
	return std::to_string(line) + "," + std::to_string(pos) + " " +
		::ToString(type) + " " + (text.empty() ? std::to_string(value) : ("\"" + text + "\""));
}
