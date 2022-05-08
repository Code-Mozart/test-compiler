#include "Parser.h"

using namespace AST;

static byte GetOpPrecedence(const char op) {
	switch (op) {
	case '=':	return 1;
	case '<':	return 1;
	case '>':	return 1;

	case '+':	return 2;
	case '-':	return 2;

	case '*':	return 3;
	case '/':	return 3;
	}
	return -1;
}

static bool IsLeftAsociative(const char op) {
	switch (op) {
	case '=':	return true;
	case '<':	return true;
	case '>':	return true;

	case '+':	return true;
	case '-':	return true;
	case '*':	return true;
	case '/':	return true;
	}
	return true;
}

static bool HasHigherPrecedence(const Token& op1, const Token& op2) {
	byte op1Prec = GetOpPrecedence(op1.text[0]);
	byte op2Prec = GetOpPrecedence(op2.text[0]);

	return (op1Prec < op2Prec) || (op1Prec == op2Prec && IsLeftAsociative(op1.text[0]));
}


bool Parser::RequireToken(const Token* token, TokenType type, const string& errMsg) {
	if (!token) {
		return false;
	}
	else if (token->type != type) {
		PushErr(errMsg, token);
		return false;
	}
	else {
		return true;
	}
}

bool Parser::RequireOperator(const Token* token, ASTOperator op) {
	if (!token) return false;
	return RequireToken(token, TokenType::Operator, string() + "expected a '" + ((char)op) + "'")
		&& ((ASTOperator)token->text[0] == op);
}


void Parser::PushErr(const string& text, const Token* tkn) {
	ASSERT(tkn);
	errh.PushErr(text, *tkn);
}


Parser::Parser(Lexer& lexer, ErrorHandler& errh)
	:
	lexer(lexer),
	errh(errh)
{
}

ref<Node> Parser::BuildAST() {
	ref<Sequence> root = RefTo<Sequence>();
	root->filepath = "filepath";
	while (lexer.HasNext()) {
		auto stm = ParseStatement();
		if (!stm) return nullptr;
		root->statements.emplace_back(stm);
	}
	return root;
}

// @optimize: employ a faster method for matching the string with all keywords
Keyword Parser::ParseKeyword(const string& s) {
	if (s == Keywords::VarDecl)		return Keyword::VarDecl;
	if (s == Keywords::WhileStm)	return Keyword::WhileStm;
	if (s == Keywords::IfStm)		return Keyword::IfStm;
	if (s == Keywords::ElseStm)		return Keyword::ElseStm;
	return Keyword::None;
}

ref<Statement> Parser::ParseStatement() {
	const Token* t = lexer.Peek();
	if (!t) return nullptr;
	switch (t->type)
	{
	case TokenType::Identifier: {
		switch (ParseKeyword(t->text)) {
		case Keyword::VarDecl:		return ParseDeclaration();
		case Keyword::WhileStm:		return ParseWhile();
		case Keyword::IfStm:		return ParseIfElse();
		default: {
			// no keyword -> assignment/call

			if (!lexer.HasNext()) break;

			const Token* identTkn = t;
			const Token* nextToken = lexer.PeekNext();

			switch (nextToken->type)
			{
			case TokenType::LParen:		return ParseCall(identTkn);
			case TokenType::Operator:	return ParseAssignment(identTkn);
			default: {
				PushErr("illegal token '" + t->ToString() + "'", t);
				return nullptr;
			}
			}
		}
		}
	}
	default: {
		PushErr("illegal token '" + t->ToString() + "', expected a statement", t);
		return nullptr;
	}
	}

	throw ParseException(t);
}

ref<AST::Expression> Parser::ParseExpression(short minPrec) {
	if (lexer.HasCurrent() && lexer.Peek()->type == TokenType::LParen) {
		lexer.Advance();
		return ParseExpression(0);
	}

	auto lhs = ParsePrimary();
	if (!lhs) return nullptr;
	const Token* token = lexer.Peek();
	while (token && token->type == TokenType::Operator) {
		const ASTOperator op = (ASTOperator)token->text[0];
		const byte prec = GetOpPrecedence((char)op);
		if (prec > minPrec) {
			lexer.Advance();
			auto rhs = ParseExpression(prec);
			if (lexer.HasCurrent() && minPrec == 0 && lexer.Peek()->type == TokenType::RParen) {
				// discard rparen
				lexer.Advance();
			}

			auto binOp = CreateNode<BinaryOperator>(token);
			binOp->op = op;
			binOp->rhs = rhs;
			binOp->lhs = lhs;

			lhs = binOp;

			token = lexer.Peek();
		}
		else {
			return lhs;
		}
	}
	return lhs;
}

ref<AST::Sequence> Parser::ParseSequence() {
	const Token* token = lexer.Peek();
	auto seq = CreateNode<Sequence>(token);
	while (token && token->type != TokenType::RBrace) {
		auto stm = ParseStatement();
		if (!stm) return nullptr;
		seq->statements.emplace_back(stm);
		token = lexer.Peek();
	}
	return seq;
}


ref<AST::Declaration> Parser::ParseDeclaration() {
	const Token* keywordTkn = lexer.Peek();
	ASSERT(keywordTkn);
	ASSERT(keywordTkn->type == TokenType::Identifier && keywordTkn->text == Keywords::VarDecl);
	lexer.Advance();

	const Token* identTkn = lexer.Advance();
	if (!RequireIdentifier(identTkn)) return nullptr;

	if (!RequireOperator(ASTOperator::Equal)) return nullptr;

	auto val = ParseExpression();
	if (!val) return nullptr;

	if (!RequireSemicolon()) return nullptr;

	ref<Declaration> decl = CreateNode<Declaration>(keywordTkn);
	decl->identifier = identTkn->text;
	decl->value = val;
	return decl;
}

ref <AST::While> Parser::ParseWhile() {
	const Token* keywordTkn = lexer.Peek();
	ASSERT(keywordTkn->type == TokenType::Identifier && keywordTkn->text == Keywords::WhileStm);
	lexer.Advance();

	auto cond = ParseCondition();
	if (!cond) return nullptr;

	auto block = ParseBlock();
	if (!block) return nullptr;

	auto whileStm = CreateNode<While>(keywordTkn);
	whileStm->condition = cond;
	whileStm->body = block;
	return whileStm;
}

ref<AST::IfElse> Parser::ParseIfElse() {
	const Token* keywordTkn = lexer.Peek();
	ASSERT(keywordTkn->type == TokenType::Identifier && keywordTkn->text == Keywords::IfStm);
	lexer.Advance();

	auto cond = ParseCondition();
	if (!cond) return nullptr;

	auto ifBlock = ParseBlock();
	if (!ifBlock) return nullptr;

	auto ifStm = CreateNode<IfElse>(keywordTkn);
	ifStm->condition = cond;
	ifStm->ifBody = ifBlock;

	const Token* elseTkn = lexer.Peek();
	if (elseTkn->type == TokenType::Identifier && elseTkn->text == Keywords::ElseStm) {
		lexer.Advance();
		auto elseBlock = ParseBlock();
		if (!elseBlock) return nullptr;
		ifStm->elseBody = elseBlock;
	}

	return ifStm;
}

ref<AST::Assignment> Parser::ParseAssignment(const Token* identTkn) {
	ASSERT(identTkn->type == TokenType::Identifier);
	lexer.Advance();

	// next token is an operator, but check it anyway (to ensure it is an equal)
	if (!RequireOperator(ASTOperator::Equal)) return nullptr;

	auto val = ParseExpression();
	if (!val) return nullptr;

	if (!RequireSemicolon()) return nullptr;

	auto assignment = CreateNode<Assignment>(identTkn);
	assignment->variable = identTkn->text;
	assignment->value = val;
	return assignment;
}

// @improve: allow functions with multiple args (handle parentheses missmatch, add ',' separator)
ref<AST::Call> Parser::ParseCall(const Token* identTkn) {
	ASSERT(identTkn->type == TokenType::Identifier);
	lexer.Advance();

	// next token is a lparen
	const Token* lparenTkn = lexer.Peek();
	ASSERT(lparenTkn->type == TokenType::LParen);
	lexer.Advance();

	auto arg1 = ParseExpression();
	if (!arg1) return nullptr;

	if (!RequireToken(TokenType::RParen)) return nullptr;

	if (!RequireSemicolon()) return nullptr;

	auto call = CreateNode<Call>(identTkn);
	call->identifier = identTkn->text;
	call->args.emplace_back(arg1);
	return call;
}

ref<AST::Expression> Parser::ParsePrimary() {
	const Token* token = lexer.Peek();
	if (!token) return nullptr;
	switch (token->type) {
	case TokenType::Literal: {
		auto cnst = CreateNode<Constant>(token);
		cnst->value = token->value;
		lexer.Advance();
		return cnst;
	}
	case TokenType::Identifier: {
		// @improve: handle fn calls
		auto var = CreateNode<Variable>(token);
		var->identifier = token->text;
		lexer.Advance();
		return var;
	}
	default: {
		PushErr("expected a value (literal or variable)", token);
		return nullptr;
	}
	}
}

ref<AST::Expression> Parser::ParseCondition() {
	if (!RequireToken(TokenType::LParen)) return nullptr;

	auto expr = ParseExpression();
	if (!expr) return nullptr;

	if (!RequireToken(TokenType::RParen)) return nullptr;

	return expr;
}

// @refactor: save a symbol table with each block/scope
ref<AST::Sequence> Parser::ParseBlock() {
	if (!RequireToken(TokenType::LBrace)) return nullptr;

	auto seq = ParseSequence();
	if (!seq) return nullptr;

	if (!RequireToken(TokenType::RBrace)) return nullptr;

	return seq;
}

template<typename T>
inline ref<T> Parser::CreateNode(const Token* tkn) {
	// @optimize: use big memory block instead
	ref<T> node = RefTo<T>();
	node->filepath = tkn->filepath;
	node->line = tkn->line;
	node->pos = tkn->pos;
	return node;
}
