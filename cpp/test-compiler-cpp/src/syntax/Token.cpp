#include "Token.h"

string ToString(TokenType type)
{
	switch (type) {
	case TokenType::Identifier:		return "Identifier";
	case TokenType::Literal:		return "Literal";
	case TokenType::LParen:			return "LParen";
	case TokenType::RParen:			return "RParen";
	case TokenType::Operator:		return "Operator";
	case TokenType::LBrace:			return "LBrace";
	case TokenType::RBrace:			return "RBrace";
	case TokenType::Semicolon:		return "Semicolon";
	}
	return "error";
}

string Token::ToString() const {
	return filepath + ":" + std::to_string(line) + "," + std::to_string(pos) + " " +
		::ToString(type) + " " + (text.empty() ? std::to_string(value) : ("\"" + text + "\""));
}
