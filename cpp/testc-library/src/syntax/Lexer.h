#pragma once

#include "util/util.h"

// Interface for a lexer (lexical analyzer). The lexer analyzes source code
// and generates corresponding tokens. Each lexer is only ever attached on
// one source string.
// Create a lexer on source code by calling attach_lexer_on(...).
//
// The lexer always points on the beginning of the next token or it has
// reached the end of the source string. The lexer internally maintains a
// buffer of tokens and only returns pointers to tokens in the buffer.
// IMPORTANT: therefore the lexer must be kept alive as long as a token
// in this buffer is used!

namespace testc {

	struct Token;
	struct Compiler_Error;

	// @think: design the lexer struct
	struct Lexer;

	// @think: how to create/attach a lexer
	//Lexer* attach_lexer_on();

	// #returns: true if the lexer has reached the end of the source string
	// or false if there are more tokens to read.
	bool has_reached_end(const Lexer* lexer);

	// Peeks the next token of the attached source string without advancing
	// the lexer past this token.
	// #returns: the token it has peeked or nullptr if the lexer reached the end.
	const Token* peek(const Lexer* lexer);

	// Advances the lexer past the next token to either the next next token or to the end.
	// #returns: the token token it moved past.
	// #throws IllegalStateException: if the lexer has already reached the end
	// before advancing.
	const Token* advance(Lexer* lexer);

	// Advances the lexder past the next token and peeks the token that comes thereafter,
	// see peek() and see advance().
	// #returns: the token that comes after the next token or nullptr if the
	// next token is the last token and the lexer reaches the end after advancing.
	// #throws IllegalStateException: if the lexer has already reached the end
	// before advancing.
	const Token* advance_and_peek(Lexer* lexer);

	// Peeks the next token without advancing the lexer, see peek().
	// #returns: success and the token it peeked or an error and nullptr
	// if the lexer reached the end.
	pair<Compiler_Error, const Token*> require_peek(const Lexer* lexer);

	// Advances the lexer and peeks the then next token, see advance_and_peek().
	// #returns: success and the token it peeked or an error and nullptr
	// if the next token before advancing is the last token and the lexer
	// has reached the end after advancing.
	// #throws IllegalStateException : if the lexer has already reached the end
	// before advancing.
	pair<Compiler_Error, const Token*> require_advance_and_peek(Lexer* lexer);

}
