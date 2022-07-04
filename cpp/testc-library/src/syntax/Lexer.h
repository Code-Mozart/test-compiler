#pragma once

#include "basic/include.h"
#include "source_info.h"
#include "syntax/token.h"

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
//
//    src-code
//     v
// -> [LEXER]
//     v
//    tokens
//     v
//    [PARSER]
//     v
//    ast
//     v
//    [TYPE CHECKER]
//     v
//    [CODE GENERATOR]
//     v
//    instructions

namespace testc {

	struct Compiler_Message;

	// Wraps the result of attaching a lexer which could generate a compiler message.
	struct Attach_Lexer_Result {
		List<Owner<const Compiler_Message>> messages;
		Nullable<Owner<Lexer>> lexer;
	};

	// Wraps the result of a lexer operation that could generate a compiler message.
	struct Lexer_Result {
		List<Owner<const Compiler_Message>> messages;
		Nullable<Ref<const Token>> token;
	};

	struct Lexer {
		const Source_Info info;

		// for now the lexer pre-tokenizes the whole src string:

		const vector<Token> tokens;
		size_t head = 0;
	};

	// Creates a new lexer and attaches the source code to it.
	// #parameter info:
	//   The source code thats attached to the lexer.
	// #returns:
	//   the lexer or nullptr and a corresponding error if the lexer could not be
	//   attached to the source.
	Attach_Lexer_Result attach_lexer_on(const Source_Info& info);

	// #returns:
	//   true if the lexer has reached the end of the source string or false
	//   if there are more tokens to read.
	bool has_reached_end(Ref<const Lexer> lexer);

	// Peeks the next token of the attached source string without advancing
	// the lexer past this token.
	// #returns:
	//   the token it has peeked or nullptr if the lexer reached the end.
	Nullable<Ref<const Token>> peek(Ref<const Lexer> lexer);

	// Advances the lexer past the next token to either the next next token or to the end.
	// #returns:
	//   the token token it moved past.
	// #throws IllegalStateException:
	//   if the lexer has already reached the end before advancing.
	Ref<const Token> advance(Ref<Lexer> lexer);

	// Advances the lexder past the next token and peeks the token that comes thereafter,
	// see peek() and see advance().
	// #returns:
	//   the token that comes after the next token or nullptr if the next token is
	//   the last token and the lexer reaches the end after advancing.
	// #throws IllegalStateException:
	//   if the lexer has already reached the end before advancing.
	Nullable<Ref<const Token>> advance_and_peek(Ref<Lexer> lexer);

	// Peeks the next token without advancing the lexer, see peek().
	// #returns:
	//   the token it peeked or nullptr and a corresponding error if the lexer
	//   reached the end.
	Lexer_Result require_peek(Ref<const Lexer> lexer);

	// Advances the lexer and peeks the then next token, see advance_and_peek().
	// #returns:
	//   the token it peeked or nullptr and a corresponding error if the next token
	//   before advancing is the last token and the lexer has reached the end
	//   after advancing.
	// #throws IllegalStateException:
	//   if the lexer has already reached the end before advancing.
	Lexer_Result require_advance_and_peek(Ref<Lexer> lexer);

}
