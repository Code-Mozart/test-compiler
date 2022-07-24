#include "token.h"
#include "semantic/operator.h"
#include "semantic/keyword.h"

namespace testc {

	static const string EOF_Type_String			= "End_Of_File";
	static const string Comma_Type_String		= "Comma";
	static const string Semicolon_Type_String	= "Semicolon";
	static const string Var_Type_String			= "Keyword_Var";
	static const string Proc_Type_String		= "Keyword_Proc";
	static const string If_Type_String			= "Keyword_If";
	static const string Else_Type_String		= "Keyword_Else";
	static const string While_Type_String		= "Keyword_While";
	static const string Identifier_Type_String	= "Identifier";
	static const string Literal_Type_String		= "Literal";
	static const string Plus_Type_String		= "Plus";
	static const string Minus_Type_String		= "Minus";
	static const string Asterisk_Type_String	= "Asterisk";
	static const string Slash_Type_String		= "Slash";
	static const string Less_Than_Type_String	= "Less_Than";
	static const string Assign_Type_String		= "Assign";
	static const string LParen_Type_String		= "LParen";
	static const string RParen_Type_String		= "RParen";
	static const string LBrace_Type_String		= "LBrace";
	static const string RBrace_Type_String		= "RBrace";

	static const string EOF_String				= "EOF";
	static const string Comma_String			= ",";
	static const string Semicolon_String		= ";";
	static const string Assign_String			= "=";
	static const string LParen_String			= "(";
	static const string RParen_String			= ")";
	static const string LBrace_String			= "{";
	static const string RBrace_String			= "}";

	const string& to_string(Token_Type type) {
		switch (type) {
		case Token_Type::End_Of_File:		return EOF_Type_String;

		case Token_Type::Comma:				return Comma_Type_String;
		case Token_Type::Semicolon:			return Semicolon_Type_String;
		
		case Token_Type::Keyword_Var:		return Var_Type_String;
		case Token_Type::Keyword_Proc:		return Proc_Type_String;
		case Token_Type::Keyword_If:		return If_Type_String;
		case Token_Type::Keyword_Else:		return Else_Type_String;
		case Token_Type::Keyword_While:		return While_Type_String;
		
		case Token_Type::Identifier:		return Identifier_Type_String;
		case Token_Type::Literal:			return Literal_Type_String;
		
		case Token_Type::Plus:				return Plus_Type_String;
		case Token_Type::Minus:				return Minus_Type_String;
		case Token_Type::Asterisk:			return Asterisk_Type_String;
		case Token_Type::Slash:				return Slash_Type_String;
		case Token_Type::Less_Than:			return Less_Than_Type_String;
		case Token_Type::Assign:			return Assign_Type_String;
		case Token_Type::LParen:			return LParen_Type_String;
		case Token_Type::RParen:			return RParen_Type_String;
		case Token_Type::LBrace:			return LBrace_Type_String;
		case Token_Type::RBrace:			return RBrace_Type_String;

		default:
			MISSING_ENUM(to_string((int)type));
		}
	}

	const string& as_string(Token_Type type) {
		switch (type) {
		case Token_Type::Comma:				return Comma_String;
		case Token_Type::Semicolon:			return Semicolon_String;

		case Token_Type::Plus:				return as_string(Operator::Plus);
		case Token_Type::Minus:				return as_string(Operator::Minus);
		case Token_Type::Asterisk:			return as_string(Operator::Asterisk);
		case Token_Type::Slash:				return as_string(Operator::Slash);
		
		case Token_Type::Less_Than:			return as_string(Operator::Less_Than);
		case Token_Type::Assign:			return Assign_String;
		
		case Token_Type::LParen:			return LParen_String;
		case Token_Type::RParen:			return RParen_String;
		case Token_Type::LBrace:			return LBrace_String;
		case Token_Type::RBrace:			return RBrace_String;

		case Token_Type::End_Of_File:		return EOF_String;
		
		case Token_Type::Keyword_Var:		return as_string(Keyword::Variable_Decl);
		case Token_Type::Keyword_Proc:		return as_string(Keyword::Procedure_Decl);
		case Token_Type::Keyword_If:		return as_string(Keyword::If);
		case Token_Type::Keyword_Else:		return as_string(Keyword::Else);
		case Token_Type::Keyword_While:		return as_string(Keyword::While);
		
		case Token_Type::Identifier:
		case Token_Type::Literal:
			throw Not_Supported_Exception(to_string(type));
		default:
			MISSING_ENUM(to_string(type));
		}
	}

	string to_string(const Token& token) {
		return as_string(token.location) + " " + to_string(token.type);
	}

}
