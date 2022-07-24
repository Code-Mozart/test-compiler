#include "lexer.h"
#include "common/compiler_message.h"
#include "basic/memory.h"

namespace testc {

    // STATIC PROCEDURE DECLARATIONS

    static List<Owner<const Token>> tokenize(Ref<const Source_Info> info);


    // HEADER PROCEDURE IMPLEMENTATIONS

    Attach_Lexer_Result attach_lexer_on(const Source_Info& info) {
        return { {}, new (allocate<Lexer>()) Lexer{ info, tokenize(&info) } };
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

    Lexer_Result require_peek(Ref<const Lexer> lexer) {
        NOT_IMPL();
    }

    Lexer_Result require_advance_and_peek(Ref<Lexer> lexer) {
        NOT_IMPL();
    }


    // STATIC PROCEDURE IMPLEMENTATIONS

    List<Owner<const Token>> tokenize(Ref<const Source_Info> info) {
        //NOT_IMPL();
        return {};
    }

}
