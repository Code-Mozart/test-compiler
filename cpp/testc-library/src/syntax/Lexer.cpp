#include "lexer.h"
#include "common/compiler_message.h"
#include "basic/memory.h"
#include "semantic/keyword.h"

namespace testc {

    static const List<char> delimiters = {
        ' ', '\n', '\t',
        ';', ',',
        '(', ')', '{', '}',
        '=', '<', /* '>', */
        '+', '-', '*', '/'  
    };


    // STRUCT DEFINITIONS

    struct Tokenize_Result {
        List<Owner<const Compiler_Message>> messages;
        List<Owner<const Token>> tokens;
    };

    // STATIC PROCEDURE DECLARATIONS

    static Tokenize_Result tokenize(Ref<const Source_Info> info);
    static Owner<const Compiler_Message> raise_illegal_char_error(
        Ref<const File> file, ulong line, ulong pos, char c, const string& msg = "");
    static int match_keyword(const String_View& text);
    static Token_Type keyword_to_token_type(Keyword k);


    // HEADER PROCEDURE IMPLEMENTATIONS

    Attach_Lexer_Result attach_lexer_on(const Source_Info& info) {
        auto [messages, tokens] = tokenize(&info);
        return { std::move(messages), new (allocate<Lexer>()) Lexer{ info, std::move(tokens) } };
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

    inline Owner<const Compiler_Message> raise_illegal_char_error(
            Ref<const File> file, ulong line, ulong pos, char c, const string& msg) {
        return raise_error(string("illegal char '") + c + "'" + msg, file, line, pos);
    }

    inline static bool is_int(char c) { return c >= '0' && c <= '9'; }
    inline static bool is_valid_first_char(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }
    inline static bool is_valid_char(char c) { return is_valid_first_char(c) || is_int(c); }

    int match_keyword(const String_View& text) {
        const List<Keyword>& keywords = get_keywords_ordered();
        int index = 0;
        int cursor = 0;
        Ref<const string> keyword = &as_string(keywords[index]);
        size_t keyword_len = keyword->length();
        for (int i = 0; i < text.len; i++) {
            char c = text.pStart[i];
            while (c != (*keyword)[cursor] && cursor < keyword_len) {
                index++;
                if (index >= keywords.size()) {
                    return -1;
                }
                else {
                    keyword = &as_string(keywords[index]);
                    keyword_len = keyword->length();
                }
            }
            cursor++;
        }
        return (int)keywords[index];
    }

    Token_Type keyword_to_token_type(Keyword k) {
        switch (k) {
        case Keyword::Else:             return Token_Type::Keyword_Else;
        case Keyword::If:               return Token_Type::Keyword_If;
        case Keyword::Procedure_Decl:   return Token_Type::Keyword_Proc;
        case Keyword::Variable_Decl:    return Token_Type::Keyword_Var;
        case Keyword::While:            return Token_Type::Keyword_While;
        }
        MISSING_ENUM(to_string(k));
    }

    Tokenize_Result tokenize(Ref<const Source_Info> info) {
        const Ref<const char> src = info->source_string.pStart;
        const size_t src_len = info->source_string.len;
        ulong line = 1;
        ulong pos = 0;
        enum {
            Comment     = 0x1 << 0,
            Skip_Line   = 0x1 << 1,
            Int_Literal = 0x1 << 2,
            Delim_Match = 0x1 << 3
        };
        byte flags = 0x0;
        Owner<Token> token = construct<Token>();
        Tokenize_Result result;

        if (src_len == 0) {
            return result;
        }

        for (int i = 0; i < src_len; i++) {
            char c = src[i];
            pos++;
            if (c == '\r') {
                continue;
            }

            if (GET_BIT(flags, Skip_Line)) {
                if (c == '\n') {
                    CLEAR_BIT(flags, Skip_Line);
                    line++;
                    pos = 0;
                }
                continue;
            }
            else if (GET_BIT(flags, Comment)) {
                if (src[i - 1] == '}' && c == '/') {
                    CLEAR_BIT(flags, Comment);
                }
                continue;
            }
            else if (c == '/') {
                if (i >= src_len - 1) {
                    result.messages.emplace_back(raise_error("unexpected EOF", info->filepath, line, pos));
                    return result;
                }
                else if (src[i + 1] == '/') {
                    SET_BIT(flags, Skip_Line);
                    i++;
                    continue;
                }
                else if (src[i + 1] == '{') {
                    SET_BIT(flags, Comment);
                    i++;
                    continue;
                }
            }

            CLEAR_BIT(flags, Delim_Match);
            for (char delim : delimiters) {
                if (c == delim) {
                    SET_BIT(flags, Delim_Match);
                    break;
                }
            }
            if (GET_BIT(flags, Delim_Match)) {
                // A delimiter matched, so push the current token
                token->location.filepath = info->filepath;
                token->location.line = line;
                token->location.pos = pos - token->text.len;

                if (GET_BIT(flags, Int_Literal)) {
                    token->type = Token_Type::Literal;
                    result.tokens.emplace_back(token);
                    token = construct<Token>();
                    CLEAR_BIT(flags, Int_Literal);
                }
                else if (token->text.len > 0) {
                    int keyword_index = match_keyword(token->text);
                    token->type = (keyword_index == -1) ?
                        Token_Type::Identifier
                        : keyword_to_token_type((Keyword)keyword_index);
                    result.tokens.emplace_back(token);
                    token = construct<Token>();
                }

                // Now push the delimiter token
                Token_Type type;
                switch (c) {
                case ';':   type = Token_Type::Semicolon;       break;
                case ',':   type = Token_Type::Comma;           break;

                case '(':   type = Token_Type::LParen;          break;
                case ')':   type = Token_Type::RParen;          break;
                case '{':   type = Token_Type::LBrace;          break;
                case '}':   type = Token_Type::RBrace;          break;

                case '+':   type = Token_Type::Plus;            break;
                case '-':   type = Token_Type::Minus;           break;
                case '*':   type = Token_Type::Asterisk;        break;
                case '/':   type = Token_Type::Slash;           break;

                case '=':   type = Token_Type::Assign;          break;
                case '<':   type = Token_Type::Less_Than;       break;
                // case '>':   type = Token_Type::Greater_Than;    break;

                case '\n':  line++; pos = 0; continue;

                default:    continue;              
                }
                result.tokens.emplace_back(new (allocate<Token>()) Token{
                    { info->filepath, line, pos },
                    type,
                    { &(src[i]), 1 }
                });
            }
            else {
                // No delimiter matched, so append the char
                if (GET_BIT(flags, Int_Literal)) {
                    if (!is_int(c)) {
                        result.messages.emplace_back(raise_illegal_char_error(
                            info->filepath, line, pos, c, ", expected a digit between '0' and '9'"));
                        return result;
                    }
                    int digit = (c - '0');
                    int new_value = ((int)token->value) * 10 + digit;
                    if (new_value > SHRT_MIN) {
                        result.messages.emplace_back(raise_error(
                            "integer overflow", info->filepath, line, pos));
                        return result;
                    }
                    token->value = new_value;
                    token->text.len++;
                }
                else if (token->text.len > 0) {
                    if (is_valid_char(c)) {
                        token->text.len++;
                    }
                    else {
                        result.messages.emplace_back(raise_illegal_char_error(info->filepath, line, pos, c));
                        return result;
                    }
                }
                else {
                    token->text.pStart = &(src[i]);
                    token->text.len = 1;
                    if (is_int(c)) {
                        token->value = (c - '0');
                        SET_BIT(flags, Int_Literal);
                    }
                    else if (is_valid_first_char(c)) {
                    }
                    else {
                        result.messages.emplace_back(raise_illegal_char_error(info->filepath, line, pos, c));
                        return result;
                    }
                }
            }
        }

        return result;
    }

}
