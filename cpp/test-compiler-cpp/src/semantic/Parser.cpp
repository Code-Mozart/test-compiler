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



Parser::Parser(const FileInfo& srcInfo, Lexer& lexer, ErrorHandler& errh)
	:
	srcInfo(srcInfo),
	lexer(lexer),
	errh(errh)
{
}

ref<Container> Parser::BuildAST(SymbolTable* builtIns) {
	auto cont = RefTo<Container>();
	cont->filepath = srcInfo.filename;
	cont->symbols = RefTo<SymbolTable>(builtIns);
	while (lexer.HasCurrent()) {
		const Token* t = lexer.Peek();
		if (t && t->type == TokenType::Identifier && t->text == Keywords::ProcDecl) {
			auto proc = ParseProcedure(cont->symbols.get());
			if (!proc) return nullptr;
			cont->procedures.emplace_back(proc);
		}
		else {
			PushErr("expected a procedure declaration", t);
			return nullptr;
		}
	}
	return cont;
}



// @optimize: employ a faster method for matching the string with all keywords
Keyword Parser::ParseKeyword(const string& s) {
	if (s == Keywords::ProcDecl)	return Keyword::ProcDecl;
	if (s == Keywords::VarDecl)		return Keyword::VarDecl;
	if (s == Keywords::WhileStm)	return Keyword::WhileStm;
	if (s == Keywords::IfStm)		return Keyword::IfStm;
	if (s == Keywords::ElseStm)		return Keyword::ElseStm;
	return Keyword::None;
}

ref<Statement> Parser::ParseStatement(SymbolTable* symbols) {
	const Token* t = lexer.Peek();
	if (!t) return nullptr;
	switch (t->type)
	{
	case TokenType::Identifier: {
		switch (ParseKeyword(t->text)) {
		case Keyword::ProcDecl: {
			PushErr("procedure declaration is not allowed here, expected a statement", t);
			return nullptr;
		}
		case Keyword::VarDecl:		return ParseDeclaration(symbols);
		case Keyword::WhileStm:		return ParseWhile(symbols);
		case Keyword::IfStm:		return ParseIfElse(symbols);
		default: {
			// no keyword -> assignment/call

			if (!lexer.HasNext()) break;

			const Token* identTkn = t;
			const Token* nextToken = lexer.PeekNext();

			switch (nextToken->type)
			{
			case TokenType::LParen:		return ParseCall(symbols, identTkn);
			case TokenType::Operator:	return ParseAssignment(symbols, identTkn);
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

ref<AST::Expression> Parser::ParseExpression(SymbolTable* symbols, short minPrec) {
	if (lexer.HasCurrent() && lexer.Peek()->type == TokenType::LParen) {
		lexer.Advance();
		return ParseExpression(symbols, 0);
	}

	auto lhs = ParsePrimary(symbols);
	if (!lhs) return nullptr;
	const Token* token = lexer.Peek();
	while (token && token->type == TokenType::Operator) {
		const ASTOperator op = (ASTOperator)token->text[0];
		const byte prec = GetOpPrecedence((char)op);
		if (prec > minPrec) {
			lexer.Advance();
			auto rhs = ParseExpression(symbols, prec);
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

ref<AST::Sequence> Parser::ParseSequence(SymbolTable* symbols) {
	const Token* token = lexer.Peek();
	auto seq = CreateNode<Sequence>(token);
	seq->symbols = RefTo<SymbolTable>(symbols);
	while (token && token->type != TokenType::RBrace) {
		auto stm = ParseStatement(seq->symbols.get());
		if (!stm) return nullptr;
		seq->statements.emplace_back(stm);
		token = lexer.Peek();
	}
	return seq;
}

ref<AST::Procedure> Parser::ParseProcedure(SymbolTable* symbols) {
	const Token* keywordTkn = lexer.Peek();
	ASSERT(keywordTkn);
	ASSERT(keywordTkn->type == TokenType::Identifier && keywordTkn->text == Keywords::ProcDecl);
	lexer.Advance();

	const Token* identTkn = lexer.Advance();
	if (!RequireIdentifier(identTkn)) return nullptr;

	if (!RequireToken(TokenType::LParen)) return nullptr;
	if (!RequireToken(TokenType::RParen)) return nullptr;

	auto proc = CreateNode<Procedure>(keywordTkn);
	proc->identifier = identTkn->text;

	// add symbol before block to allow for recursion
	ASSERT(symbols);
	auto conflictingDef = symbols->AddProc(proc);
	if (conflictingDef) {
		PushErr("procedure definition '" + proc->identifier + "' conflicts with "
			+ conflictingDef->Node::ToString(), identTkn);
		return nullptr;
	}

	auto block = ParseBlock(symbols);
	if (!block) return nullptr;
	proc->body = block;

	return proc;
}



ref<AST::Declaration> Parser::ParseDeclaration(SymbolTable* symbols) {
	const Token* keywordTkn = lexer.Peek();
	ASSERT(keywordTkn);
	ASSERT(keywordTkn->type == TokenType::Identifier && keywordTkn->text == Keywords::VarDecl);
	lexer.Advance();

	const Token* identTkn = lexer.Advance();
	if (!RequireIdentifier(identTkn)) return nullptr;

	if (!RequireOperator(ASTOperator::Equal)) return nullptr;

	auto val = ParseExpression(symbols);
	if (!val) return nullptr;

	if (!RequireSemicolon()) return nullptr;

	ref<Declaration> decl = CreateNode<Declaration>(keywordTkn);
	decl->identifier = identTkn->text;
	decl->value = val;
	
	// add symbol after parsing the expressions value,
	// because it is not known at this point
	ASSERT(symbols);
	auto conflictingDecl = symbols->AddVar(decl);
	if (conflictingDecl) {
		PushErr("variable declaration '" + decl->identifier + "' conflicts with "
			+ conflictingDecl->Node::ToString(), identTkn);
		return nullptr;
	}

	return decl;
}

ref <AST::While> Parser::ParseWhile(SymbolTable* symbols) {
	const Token* keywordTkn = lexer.Peek();
	ASSERT(keywordTkn);
	ASSERT(keywordTkn->type == TokenType::Identifier && keywordTkn->text == Keywords::WhileStm);
	lexer.Advance();

	auto cond = ParseCondition(symbols);
	if (!cond) return nullptr;

	auto block = ParseBlock(symbols);
	if (!block) return nullptr;

	auto whileStm = CreateNode<While>(keywordTkn);
	whileStm->condition = cond;
	whileStm->body = block;
	return whileStm;
}

ref<AST::IfElse> Parser::ParseIfElse(SymbolTable* symbols) {
	const Token* keywordTkn = lexer.Peek();
	ASSERT(keywordTkn->type == TokenType::Identifier && keywordTkn->text == Keywords::IfStm);
	lexer.Advance();

	auto cond = ParseCondition(symbols);
	if (!cond) return nullptr;

	auto ifBlock = ParseBlock(symbols);
	if (!ifBlock) return nullptr;

	auto ifStm = CreateNode<IfElse>(keywordTkn);
	ifStm->condition = cond;
	ifStm->ifBody = ifBlock;

	const Token* elseTkn = lexer.Peek();
	if (elseTkn->type == TokenType::Identifier && elseTkn->text == Keywords::ElseStm) {
		lexer.Advance();
		auto elseBlock = ParseBlock(symbols);
		if (!elseBlock) return nullptr;
		ifStm->elseBody = elseBlock;
	}

	return ifStm;
}

ref<AST::Assignment> Parser::ParseAssignment(SymbolTable* symbols, const Token* identTkn) {
	ASSERT(identTkn->type == TokenType::Identifier);
	lexer.Advance();

	// next token is an operator, but check it anyway (to ensure it is an equal)
	if (!RequireOperator(ASTOperator::Equal)) return nullptr;

	auto val = ParseExpression(symbols);
	if (!val) return nullptr;

	if (!RequireSemicolon()) return nullptr;

	auto assignment = CreateNode<Assignment>(identTkn);
	assignment->variable = identTkn->text;
	assignment->value = val;
	return assignment;
}

// @improve: allow functions with multiple args (handle parentheses missmatch, add ',' separator)
ref<AST::Call> Parser::ParseCall(SymbolTable* symbols, const Token* identTkn) {
	ASSERT(identTkn->type == TokenType::Identifier);
	lexer.Advance();

	auto call = CreateNode<Call>(identTkn);
	call->identifier = identTkn->text;

	ASSERT(symbols);
	symbols->AddCall(call);

	// next token is a lparen
	const Token* lparenTkn = lexer.Peek();
	ASSERT(lparenTkn->type == TokenType::LParen);
	lexer.Advance();

	if (lexer.HasCurrent() && lexer.Peek()->type != TokenType::RParen) {
		auto arg1 = ParseExpression(symbols);
		if (!arg1) return nullptr;

		call->args.emplace_back(arg1);
	}

	if (!RequireToken(TokenType::RParen)) return nullptr;

	if (!RequireSemicolon()) return nullptr;

	return call;
}

ref<AST::Expression> Parser::ParsePrimary(SymbolTable* symbols) {
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

		ASSERT(symbols);
		if (!symbols->GetVar(token->text)) {
			PushErr("variable '" + token->text + "' has not yet been declared", token);
			return nullptr;
		}

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

ref<AST::Expression> Parser::ParseCondition(SymbolTable* symbols) {
	if (!RequireToken(TokenType::LParen)) return nullptr;

	auto expr = ParseExpression(symbols);
	if (!expr) return nullptr;

	if (!RequireToken(TokenType::RParen)) return nullptr;

	return expr;
}

ref<AST::Sequence> Parser::ParseBlock(SymbolTable* symbols) {
	if (!RequireToken(TokenType::LBrace)) return nullptr;

	auto seq = ParseSequence(symbols);
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
