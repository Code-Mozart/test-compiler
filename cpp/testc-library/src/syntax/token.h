#pragma once

#include "util/util.h"
#include "util/string_view.h"
#include "common/location.h"
#include "semantic/operator.h"

namespace testc {

	enum class Token_Type {
		End_Of_File,

		Comma,
		Semicolon,

		Keyword_Var,
		Keyword_Proc,
		Keyword_If,
		Keyword_Else,
		Keyword_While,
		
		Identifier,
		Literal,

		Plus,
		Minus,
		Asterisk,
		Slash,

		Less_Than,
		Assign,

		LParen, RParen,
		LBrace, RBrace
	};

	struct Token {
		Location location;
		Token_Type type;
		String_View text;
		union {
			short value;
			Operator op;
		};
	};

	const string& to_string(Token_Type type);
	const string& as_string(Token_Type type);
	const string& to_string(const Token& token);

}
