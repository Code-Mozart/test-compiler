#include "token.h"
#include "exception.h"

namespace testc {

	string to_string(Token_Type type) {
		switch (type) {
		case Token_Type::End_Of_File:		return "End_Of_File";

		case Token_Type::Comma:				return "Comma";
		case Token_Type::Semicolon:			return "Semicolon";
		
		case Token_Type::Keyword_Var:		return "Keyword_Var";
		case Token_Type::Keyword_Proc:		return "Keyword_Proc";
		case Token_Type::Keyword_If:		return "Keyword_If";
		case Token_Type::Keyword_Else:		return "Keyword_Else";
		case Token_Type::Keyword_While:		return "Keyword_While";
		
		case Token_Type::Identifier:		return "Identifier";
		case Token_Type::Literal:			return "Literal";
		
		case Token_Type::Plus:				return "Plus";
		case Token_Type::Minus:				return "Minus";
		case Token_Type::Asterisk:			return "Asterisk";
		case Token_Type::Slash:				return "Slash";
		
		case Token_Type::Less_Than:			return "Less_Than";
		case Token_Type::Assign:			return "Assign";
		
		case Token_Type::LParen:			return "LParen";
		case Token_Type::RParen:			return "RParen";
		case Token_Type::LBrace:			return "LBrace";
		case Token_Type::RBrace:			return "RBrace";
		}
	}

	string as_string(Token_Type type) {
		switch (type) {
		case Token_Type::Comma:				return ",";
		case Token_Type::Semicolon:			return ";";

		case Token_Type::Plus:				return "+";
		case Token_Type::Minus:				return "-";
		case Token_Type::Asterisk:			return "*";
		case Token_Type::Slash:				return "/";
		
		case Token_Type::Less_Than:			return "<";
		case Token_Type::Assign:			return "=";
		
		case Token_Type::LParen:			return "(";
		case Token_Type::RParen:			return ")";
		case Token_Type::LBrace:			return "{";
		case Token_Type::RBrace:			return "}";

		case Token_Type::End_Of_File:		return "EOF";
		
		case Token_Type::Keyword_Var:		return "var";
		case Token_Type::Keyword_Proc:		return "proc";
		case Token_Type::Keyword_If:		return "if";
		case Token_Type::Keyword_Else:		return "else";
		case Token_Type::Keyword_While:		return "while";
		
		case Token_Type::Identifier:
		case Token_Type::Literal:
			throw Not_Supported_Exception(to_string(type));
		default:
			MISSING_ENUM(to_string(type));
		}
	}

	string to_string(const Token& token) {
		NOT_IMPL();
	}

}
