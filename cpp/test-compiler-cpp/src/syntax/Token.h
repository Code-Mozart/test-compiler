#pragma once

#include "util/Util.h"

enum class TokenType {
	Identifier,
	Literal,
	LParen, RParen, Operator,
	LBrace, RBrace, Semicolon
};

string ToString(TokenType type);

class Token {
public:
	string filepath; // @optimize: use a ptr instead of a copy
	ulong line;
	ulong pos;

	TokenType type;
	string text; // @optimize: use a view on the string instead of a copy
	short value;
public:
	Token(string filepath, ulong line, ulong pos, TokenType type, string text, short value)
		: filepath(filepath), line(line), pos(pos), type(type), text(text), value(value) {}

	string ToString() const;
};
