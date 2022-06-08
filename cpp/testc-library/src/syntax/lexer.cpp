#include "lexer.h"
#include "exception.h"
#include "common/compiler_message.h"

namespace testc {

    // STATIC PROCEDURE DECLARATIONS

    static vector<Token> tokenize(Ref<const Source_Info> info);


    // HEADER PROCEDURE IMPLEMENTATIONS

    Owner<Lexer> attach_lexer_on(const Source_Info& info) {
        return new Lexer{ info, tokenize(&info) };
    }

    bool has_reached_end(Ref<const Lexer> lexer) {
        NOT_IMPL();
    }

	Nullable<Ref<const Token>> peek(Ref<const Lexer> lexer) {
        NOT_IMPL();
    }

	Ref<const Token> advance(Ref<Lexer> lexer) {
        NOT_IMPL();
    }

    Nullable<Ref<const Token>> advance_and_peek(Ref<Lexer> lexer) {
        NOT_IMPL();
    }

    pair<Compiler_Message, Nullable<Ref<const Token>>> require_peek(Ref<const Lexer> lexer) {
        NOT_IMPL();
    }

    pair<Compiler_Message, Nullable<Ref<const Token>>> require_advance_and_peek(Ref<Lexer> lexer) {
        NOT_IMPL();
    }


    // STATIC PROCEDURE IMPLEMENTATIONS

    vector<Token> tokenize(Ref<const Source_Info> info) {
        NOT_IMPL();
    }

}
