#include "lexer.h"
#include "exception.h"
#include "common/compiler_message.h"

namespace testc {

    Owner<Lexer> attach_lexer_on(const Source_Info& info) {
        NOT_IMPL();
    }

    bool has_reached_end(const Ref<Lexer> lexer) {
        NOT_IMPL();
    }

	const Ref<Token> peek(const Ref<Lexer> lexer) {
        NOT_IMPL();
    }

	const Ref<Token> advance(Ref<Lexer> lexer) {
        NOT_IMPL();
    }

    const Ref<Token> advance_and_peek(Ref<Lexer> lexer) {
        NOT_IMPL();
    }

    pair<Compiler_Message, const Ref<Token>> require_peek(const Ref<Lexer> lexer) {
        NOT_IMPL();
    }

    pair<Compiler_Message, const Ref<Token>> require_advance_and_peek(Ref<Lexer> lexer) {
        NOT_IMPL();
    }

}
