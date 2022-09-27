#include "Parser.h"
#include "basic/memory.h"
#include "util/list_util.h"
#include "common/result.h"
#include "syntax/Lexer.h"
#include "syntax/ast.h"
#include "semantic/symbol_table.h"

namespace testc {

    // STRUCT DEFINITIONS

    struct Peek_Result : public Result {
        Nullable<Ref<const Token>> token;
    };


    // STATIC PROCEDURE DECLARATIONS

    Peek_Result require_token(Token_Type type, Ref<Lexer> lexer);
    
    template<typename T>
    Owner<T> create_node(Ref<Symbol_Table> symbols);

    Parser_Result<AST_Statement> parse_identifier_statement(Ref<Lexer> lexer, Ref<Symbol_Table> symbols);
    Parser_Result<AST_Call> parse_call(
        Ref<const Token> identifier_token, Ref<Lexer> lexer, Ref<Symbol_Table> symbols);
    Parser_Result<AST_Assignment> parse_assignment(
        Ref<const Token> identifier_token, Ref<Lexer> lexer, Ref<Symbol_Table> symbols);


    // HEADER PROCEDURE IMPLEMENTATIONS
    
	Owner<Symbol_Table> create_symbol_table(Nullable<Ref<Symbol_Table>> parent) {
        Owner<Symbol_Table> table = new (allocate<Symbol_Table>()) Symbol_Table{
            nullptr,
            {},
            (parent) ? parent->counter++ : 0,
            0,
            {}, {}, {}
        };
        if (parent) {
            parent->children.emplace_back(table);
        }
        return table;
    }

    // Declaration scope
    
	Parser_Result_Multi<AST_Procedure> parse_procedures(Ref<Lexer> lexer, Ref<Symbol_Table> symbols) {
        Parser_Result_Multi<AST_Procedure> result;
        while (!has_reached_end(lexer)) {
            Parser_Result<AST_Procedure> single_result = parse_procedure(lexer, symbols);
            move_elements(single_result.messages, result.messages);
            result.nodes.emplace_back(single_result.node);
        }
        return result;
    }
    

    // Procedures

    Parser_Result<AST_Procedure> parse_procedure(Ref<Lexer> lexer, Ref<Symbol_Table> symbols) {
        Parser_Result<AST_Procedure> result;
        Owner<AST_Procedure> proc = create_node<AST_Procedure>(symbols);

        // require the procedure keyword and set the node location
        Peek_Result r = require_token(Token_Type::Keyword_Proc, lexer);
        move_elements(r.messages, result.messages);
        if (has_failed(r)) {
            destroy(proc);
            return result;
        }
        else {
            proc->location = r.token->location;
        }
        advance(lexer);

        // require an identifier and use it for the symbol
        r = require_token(Token_Type::Identifier, lexer);
        move_elements(r.messages, result.messages);
        if (has_failed(r)) {
            destroy(proc);
            return result;
        }
        else {
            proc->identifier = r.token->text;
        }
        advance(lexer);

        // require a '('
        r = require_token(Token_Type::LParen, lexer);
        move_elements(r.messages, result.messages);
        if (has_failed(r)) {
            destroy(proc);
            return result;
        }
        advance(lexer);

        // parse the procedure parameters
        Owner<Symbol_Table> param_symbols = create_symbol_table(symbols);
        Lexer_Result lr = require_peek(lexer);
        move_elements(lr.messages, result.messages);
        if (has_failed(lr)) {
            destroy(param_symbols);
            destroy(proc);
            return result;
        }
        while (lr.token->type != Token_Type::RParen) {
            // require a ',' after the first parameter is read
            if (!proc->parameters.empty()) {
                r = require_token(Token_Type::Comma, lexer);
                move_elements(r.messages, result.messages);
                if (has_failed(r)) {
                    destroy(param_symbols);
                    destroy(proc);
                    return result;
                }
                advance(lexer);
            }

            Owner<AST_Declaration> param = create_node<AST_Declaration>(symbols);

            // require the variable keyword and set the node location
            r = require_token(Token_Type::Keyword_Var, lexer);
            move_elements(r.messages, result.messages);
            if (has_failed(r)) {
                destroy(param_symbols);
                destroy(proc);
                return result;
            }
            else {
                param->location = r.token->location;
            }
            advance(lexer);

            // require the parameter identifier
            r = require_token(Token_Type::Identifier, lexer);
            move_elements(r.messages, result.messages);
            if (has_failed(r)) {                
                destroy(param_symbols);
                destroy(proc);
                return result;
            }
            else {
                param->identifier = r.token->text;
            }
            advance(lexer);

            // add the symbol to the parameter list
            Result add_sym_result = add_value_symbol(param_symbols, param->identifier, param);
            move_elements(add_sym_result.messages, result.messages);
            if (has_failed(add_sym_result)) {
                destroy(param_symbols);
                destroy(proc);
                return result;
            }
            else {
                proc->parameters.emplace_back(param);
            }

            Lexer_Result lr = require_peek(lexer);
            move_elements(lr.messages, result.messages);
            if (has_failed(lr)) {
                destroy(param_symbols);
                destroy(proc);
                return result;
            }
        }

        // consume the ')' token
        lr = require_advance_and_peek(lexer);
        ASSERT(!has_failed(lr));

        // parse the procedure body
        Parser_Result<AST_Block> parse_body_result = parse_block(lexer, param_symbols);
        move_elements(parse_body_result.messages, result.messages);
        if (has_failed(parse_body_result)) {
            destroy(param_symbols);
            destroy(proc);
            return result;
        }
        Owner<AST_Block> body = std::move(parse_body_result.node);
        proc->body = body;

        result.node = proc;
        return result;
    }


    // Statements

	Parser_Result<AST_Statement> parse_statement(Ref<Lexer> lexer, Ref<Symbol_Table> symbols) {
        NOT_IMPL();
    }

	Parser_Result_Multi<AST_Statement> parse_statements(Ref<Lexer> lexer, Ref<Symbol_Table> symbols) {
        Parser_Result_Multi<AST_Statement> result;

        while (true) {
            Lexer_Result lr = require_peek(lexer);
            if (has_failed(lr)) {
                destroy_elements(result.nodes);
                return result;
            }

            switch (lr.token->type) {
            case Token_Type::LBrace: {
                Parser_Result<AST_Block> pr = parse_block(lexer, symbols);
                move_elements(lr.messages, result.messages);
                move_elements(pr.messages, result.messages);
                if (has_failed(pr)) {
                    destroy_elements(result.nodes);
                    return result;
                }
                else {
                    result.nodes.emplace_back(std::move(pr.node));
                }
                break;
            }
            case Token_Type::Keyword_While: {
                Parser_Result<AST_While> pr = parse_while(lexer, symbols);
                move_elements(lr.messages, result.messages);
                move_elements(pr.messages, result.messages);
                if (has_failed(pr)) {
                    destroy_elements(result.nodes);
                    return result;
                }
                else {
                    result.nodes.emplace_back(std::move(pr.node));
                }
                break;
            }
            case Token_Type::Keyword_If: {
                Parser_Result<AST_If_Else> pr = parse_if_else(lexer, symbols);
                move_elements(lr.messages, result.messages);
                move_elements(pr.messages, result.messages);
                if (has_failed(pr)) {
                    destroy_elements(result.nodes);
                    return result;
                }
                else {
                    result.nodes.emplace_back(std::move(pr.node));
                }
                break;
            }
            case Token_Type::Keyword_Var: {
                Parser_Result<AST_Declaration> pr = parse_declaration(lexer, symbols);
                move_elements(lr.messages, result.messages);
                move_elements(pr.messages, result.messages);
                if (has_failed(pr)) {
                    destroy_elements(result.nodes);
                    return result;
                }
                else {
                    result.nodes.emplace_back(std::move(pr.node));
                }
                break;
            }
            case Token_Type::Identifier: {
                Parser_Result<AST_Statement> pr = parse_identifier_statement(lexer, symbols);
                move_elements(lr.messages, result.messages);
                move_elements(pr.messages, result.messages);
                if (has_failed(pr)) {
                    destroy_elements(result.nodes);
                    return result;
                }
                else {
                    result.nodes.emplace_back(std::move(pr.node));
                }
                break;
            }
            default:
                return result;
            }
        }
    }
    
	Parser_Result<AST_Block> parse_block(Ref<Lexer> lexer, Ref<Symbol_Table> symbols) {
        Parser_Result<AST_Block> result;
        Owner<AST_Block> block = create_node<AST_Block>(symbols);

        // require a '{' and set the node location
        Peek_Result r = require_token(Token_Type::LBrace, lexer);
        move_elements(r.messages, result.messages);
        if (has_failed(r)) {
            destroy(block);
            return result;
        }
        else {
            block->location = r.token->location;
        }
        advance(lexer);

        // create the block symbol table
        Owner<Symbol_Table> block_symbols = create_symbol_table(symbols);

        // parse the statements
        Parser_Result_Multi<AST_Statement> parse_stms_result = parse_statements(lexer, symbols);
        move_elements(parse_stms_result.messages, result.messages);
        if (has_failed(parse_stms_result)) {
            destroy(block_symbols);
            destroy(block);
            return result;
        }
        else {
            block->statements = std::move(parse_stms_result.nodes);
        }

        // require a '{'
        r = require_token(Token_Type::RBrace, lexer);
        move_elements(r.messages, result.messages);
        if (has_failed(r)) {
            destroy(block_symbols);
            destroy(block);
            return result;
        }
        advance(lexer);

        result.node = block;
        return result;
    }

	Parser_Result<AST_Declaration> parse_declaration(Ref<Lexer> lexer, Ref<Symbol_Table> symbols) {
        NOT_IMPL();
    }

	Parser_Result<AST_Assignment> parse_assignment(Ref<Lexer> lexer, Ref<Symbol_Table> symbols) {
        NOT_IMPL();
    }

	Parser_Result<AST_While> parse_while(Ref<Lexer> lexer, Ref<Symbol_Table> symbols) {
        NOT_IMPL();
    }

	Parser_Result<AST_If_Else> parse_if_else(Ref<Lexer> lexer, Ref<Symbol_Table> symbols) {
        NOT_IMPL();
    }

	Parser_Result<AST_Call> parse_call(Ref<Lexer> lexer, Ref<Symbol_Table> symbols) {
        NOT_IMPL();
    }


    // Expressions

	Parser_Result<AST_Expression> parse_expression(Ref<Lexer> lexer, Ref<Symbol_Table> symbols) {
        return parse_expression(-1, lexer, symbols);
    }

	Parser_Result<AST_Constant> parse_constant(Ref<Lexer> lexer, Ref<Symbol_Table> symbols) {
        NOT_IMPL();
    }

	Parser_Result<AST_Variable> parse_variable(Ref<Lexer> lexer, Ref<Symbol_Table> symbols) {
        NOT_IMPL();
    }
	
    Parser_Result<AST_Binary_Op> parse_binary_op(Ref<Lexer> lexer, Ref<Symbol_Table> symbols) {
        NOT_IMPL();
    }


    // STATIC PROCEDURE IMPLEMENTATIONS

    Peek_Result require_token(Token_Type type, Ref<Lexer> lexer) {
        Lexer_Result lr = require_peek(lexer);
        if (has_failed(lr)) {
            return Peek_Result{
                false,
                std::move(lr.messages),
                nullptr
            };
        }
        else if (lr.token->type != type) {
            Peek_Result result = Peek_Result{
                false,
                std::move(lr.messages),
                nullptr
            };
            result.messages.emplace_back(
                raise_error("expected a '" + as_string(type) + "'", lr.token->location));
            return result;
        }
        else {
            return Peek_Result{
                true,
                std::move(lr.messages),
                lr.token
            };
        }
    }

    Parser_Result<AST_Statement> parse_identifier_statement(Ref<Lexer> lexer, Ref<Symbol_Table> symbols) {
        // either an assignment or a call

        Parser_Result<AST_Statement> result;

        Lexer_Result lr_identifier = require_peek(lexer);
        ASSERT(!has_failed(lr_identifier));
        ASSERT(lr_identifier.token->type == Token_Type::Identifier);

        Lexer_Result lr_next = require_advance_and_peek(lexer);
        move_elements(lr_next.messages, result.messages);
        if (has_failed(lr_next)) {
            return result;
        }
        
        switch (lr_next.token->type) {
        case Token_Type::LParen: {
            Parser_Result<AST_Call> pr = parse_call(lr_identifier.token, lexer, symbols);
            move_elements(pr.messages, result.messages);
            if (has_failed(pr)) {
                return result;
            }
            result.node = std::move(pr.node);
            return result;
        }
        case Token_Type::Assign: {
            Parser_Result<AST_Assignment> pr = parse_assignment(lr_identifier.token, lexer, symbols);
            move_elements(pr.messages, result.messages);
            if (has_failed(pr)) {
                return result;
            }
            result.node = std::move(pr.node);
            return result;
        }
        default: {
            result.messages.emplace_back(raise_error(
                "illegal token '" + lr_next.token->text + "' of type " + to_string(lr_next.token->type)
                + ", expected a statement", lr_next.token->location));
            return result;
        }
        }
    }

    Parser_Result<AST_Call> parse_call(Ref<const Token> identifier_token,
            Ref<Lexer> lexer, Ref<Symbol_Table> symbols) {
        Parser_Result<AST_Call> result;
        Owner<AST_Call> call = create_node<AST_Call>(symbols);

        // set the identifier
        call->location = identifier_token->location;
        call->identifier = identifier_token->text;

        // require a '('
        Peek_Result r = require_token(Token_Type::LParen, lexer);
        move_elements(r.messages, result.messages);
        if (has_failed(r)) {
            destroy(call);
            return result;
        }

        // parse the call arguments
        Lexer_Result lr = require_peek(lexer);
        move_elements(lr.messages, result.messages);
        if (has_failed(lr)) {
            destroy(call);
            return result;
        }
        while (lr.token->type != Token_Type::RParen) {
            // require a ',' after the first argument is read
            if (!call->arguments.empty()) {
                r = require_token(Token_Type::Comma, lexer);
                move_elements(r.messages, result.messages);
                if (has_failed(r)) {
                    destroy(call);
                    return result;
                }
                advance(lexer);
            }

            // parse the argument
            Parser_Result<AST_Expression> pr = parse_expression(lexer, symbols);
            move_elements(pr.messages, result.messages);
            if (has_failed(pr)) {
                destroy(call);
                return result;
            }
            else {
                call->arguments.emplace_back(pr.node);
            }

            lr = require_peek(lexer);
            move_elements(lr.messages, result.messages);
            if (has_failed(lr)) {
                destroy(call);
                return result;
            }
        }

        // consume the ')' token
        lr = require_advance_and_peek(lexer);
        ASSERT(!has_failed(lr));

        // require a ';'
        r = require_token(Token_Type::Semicolon, lexer);
        move_elements(r.messages, result.messages);
        if (has_failed(r)) {
            destroy(call);
            return result;
        }

        result.node = call;
        return result;
    }

    Parser_Result<AST_Assignment> parse_assignment(Ref<const Token> identifier_token,
            Ref<Lexer> lexer, Ref<Symbol_Table> symbols) {
        NOT_IMPL();
    }
    
    // Node creation

    Owner<AST_Procedure> create_node<AST_Procedure>(Ref<Symbol_Table> symbols) {
        Owner<AST_Procedure> node = construct<AST_Procedure>();
        node->type = AST_Type::Procedure;
        node->symbols = symbols;
        return node;
    }

    Owner<AST_Declaration> create_node<AST_Declaration>(Ref<Symbol_Table> symbols) {
        Owner<AST_Declaration> node = construct<AST_Declaration>();
        node->type = AST_Type::Declaration;
        node->symbols = symbols;
        return node;
    }

    Owner<AST_Block> create_node<AST_Block>(Ref<Symbol_Table> symbols) {
        Owner<AST_Block> node = construct<AST_Block>();
        node->type = AST_Type::Block;
        node->symbols = symbols;
        return node;
    }

    Owner<AST_Call> create_node<AST_Call>(Ref<Symbol_Table> symbols) {
        Owner<AST_Call> node = construct<AST_Call>();
        node->type = AST_Type::Call;
        node->symbols = symbols;
        return node;
    }

}
