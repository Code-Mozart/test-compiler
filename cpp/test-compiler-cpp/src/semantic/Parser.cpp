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


bool Parser::RequireToken(const Token& token, TokenType type, const string& errMsg) {
	if (token.type != type) {
		PushErr(errMsg, token);
		return false;
	}
	else {
		return true;
	}
}

bool Parser::RequireOperator(const Token& token, ASTOperator op) {
	return RequireToken(token, TokenType::Operator, string() + "expected a '" + ((char)op) + "'")
		&& ((ASTOperator)token.text[0] == op);
}


void Parser::PushErr(const string& text, const Token& tkn) {
	errh.PushErr(text, tkn);
}


ref<Node> Parser::BuildAST(const vector<Token>& tokens) {
	ref<Sequence> root = RefTo<Sequence>();
	root->filepath = "filepath";
	for (int i = 0; i < tokens.size();) {
		auto stm = ParseStatement(tokens, i);
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

ref<Statement> Parser::ParseStatement(const vector<Token>& tokens, int& index)
{
	const Token& t = tokens[index];
	switch (t.type)
	{
	case TokenType::Identifier: {
		switch (ParseKeyword(t.text)) {
		case Keyword::VarDecl:		return ParseDeclaration(tokens, index);
		case Keyword::WhileStm:		return ParseWhile(tokens, index);
		case Keyword::IfStm:		return ParseIfElse(tokens, index);
		default: {
			// no keyword -> assignment/call

			const Token& identTkn = t;
			const Token& nextToken = tokens[++index];

			if (nextToken.type == TokenType::LParen)
				return ParseCall(tokens, index, identTkn);
			else if (nextToken.type == TokenType::Operator)
				return ParseAssignment(tokens, index, identTkn);
			else {
				PushErr("illegal token '" + t.ToString() + "'", t);
				return nullptr;
			}
		}
		}
	}
	default: {
		PushErr("illegal token '" + t.ToString() + "', expected a statement", t);
		return nullptr;
	}
	}

	throw ParseException(t);
}

ref<AST::Expression> Parser::ParseExpression(const vector<Token>& tokens, int& index, short minPrec) {
	if (tokens[index].type == TokenType::LParen) {
		index++;
		return ParseExpression(tokens, index, 0);
	}

	auto lhs = ParsePrimary(tokens, index);
	if (!lhs) return nullptr;
	const Token* next = &(tokens[index]);
	while (next->type == TokenType::Operator) {
		const ASTOperator op = (ASTOperator)next->text[0];
		const byte prec = GetOpPrecedence((char)op);
		if (prec > minPrec) {
			index++;
			auto rhs = ParseExpression(tokens, index, prec);
			if (minPrec == 0 && tokens[index].type == TokenType::RParen) {
				// discard rparen
				index++;
			}

			auto binOp = CreateNode<BinaryOperator>(*next);
			binOp->op = op;
			binOp->rhs = rhs;
			binOp->lhs = lhs;
			lhs = binOp;

			next = &(tokens[index]);
		}
		else {
			return lhs;
		}
	}
	return lhs;
}

ref<AST::Sequence> Parser::ParseSequence(const vector<Token>& tokens, int& index) {
	const Token* next = &(tokens[index]);
	auto seq = CreateNode<Sequence>(tokens[index]);
	while (next->type != TokenType::RBrace && index < tokens.size()) {
		auto stm = ParseStatement(tokens, index);
		if (!stm) return nullptr;
		seq->statements.emplace_back(stm);
		next = &(tokens[index]);
	}
	return seq;
}


ref<AST::Declaration> Parser::ParseDeclaration(const vector<Token>& tokens, int& index) {
	const Token& keywordTkn = tokens[index];
	ASSERT(keywordTkn.type == TokenType::Identifier && keywordTkn.text == Keywords::VarDecl);
	index++;

	const Token& identTkn = tokens[index++];
	if (!RequireIdentifier(identTkn)) return nullptr;

	if (!RequireOperator(tokens, index, ASTOperator::Equal)) return nullptr;

	auto val = ParseExpression(tokens, index);
	if (!val) return nullptr;

	if (!RequireSemicolon(tokens, index)) return nullptr;

	ref<Declaration> decl = CreateNode<Declaration>(keywordTkn);
	decl->identifier = identTkn.text;
	decl->value = val;
	return decl;
}

ref <AST::While> Parser::ParseWhile(const vector<Token>& tokens, int& index) {
	const Token& keywordTkn = tokens[index];
	ASSERT(keywordTkn.type == TokenType::Identifier && keywordTkn.text == Keywords::WhileStm);
	index++;

	auto cond = ParseCondition(tokens, index);
	if (!cond) return nullptr;

	auto block = ParseBlock(tokens, index);
	if (!block) return nullptr;

	auto whileStm = CreateNode<While>(keywordTkn);
	whileStm->condition = cond;
	whileStm->body = block;
	return whileStm;
}

ref<AST::IfElse> Parser::ParseIfElse(const vector<Token>& tokens, int& index) {
	const Token& keywordTkn = tokens[index];
	ASSERT(keywordTkn.type == TokenType::Identifier && keywordTkn.text == Keywords::IfStm);
	index++;

	auto cond = ParseCondition(tokens, index);
	if (!cond) return nullptr;

	auto ifBlock = ParseBlock(tokens, index);
	if (!ifBlock) return nullptr;

	auto ifStm = CreateNode<IfElse>(keywordTkn);
	ifStm->condition = cond;
	ifStm->ifBody = ifBlock;

	const Token& elseTkn = tokens[index];
	if (elseTkn.type == TokenType::Identifier && elseTkn.text == Keywords::ElseStm) {
		index++;
		auto elseBlock = ParseBlock(tokens, index);
		if (!elseBlock) return nullptr;
		ifStm->elseBody = elseBlock;
	}

	return ifStm;
}

ref<AST::Assignment> Parser::ParseAssignment(const vector<Token>& tokens, int& index, const Token& identTkn) {
	ASSERT(identTkn.type == TokenType::Identifier);

	// next token is an operator, but check it anyway
	if (!RequireOperator(tokens, index, ASTOperator::Equal)) return nullptr;

	auto val = ParseExpression(tokens, index);
	if (!val) return nullptr;

	if (!RequireSemicolon(tokens, index)) return nullptr;

	auto assignment = CreateNode<Assignment>(identTkn);
	assignment->variable = identTkn.text;
	assignment->value = val;
	return assignment;
}

// @improve: allow functions with multiple args (handle parentheses missmatch, add ',' separator)
ref<AST::Call> Parser::ParseCall(const vector<Token>& tokens, int& index, const Token& identTkn) {
	ASSERT(identTkn.type == TokenType::Identifier);

	// next token is a lparen
	const Token& lparenTkn = tokens[index++];
	ASSERT(lparenTkn.type == TokenType::LParen);

	auto arg1 = ParseExpression(tokens, index);
	if (!arg1) return nullptr;

	if (!RequireToken(tokens, index, TokenType::RParen)) return nullptr;

	auto call = CreateNode<Call>(identTkn);
	call->identifier = identTkn.text;
	call->args.emplace_back(arg1);

	if (!RequireSemicolon(tokens, index)) return nullptr;

	return call;
}

ref<AST::Expression> Parser::ParsePrimary(const vector<Token>& tokens, int& index) {
	const Token& tkn = tokens[index++];
	switch (tkn.type) {
	case TokenType::Literal: {
		auto cnst = CreateNode<Constant>(tkn);
		cnst->value = tkn.value;
		return cnst;
	}
	case TokenType::Identifier: {
		// @improve: handle fn calls
		auto var = CreateNode<Variable>(tkn);
		var->identifier = tkn.text;
		return var;
	}
	default: {
		PushErr("expected a value (literal or variable)", tkn);
		return nullptr;
	}
	}
}

ref<AST::Expression> Parser::ParseCondition(const vector<Token>& tokens, int& index) {
	if (!RequireToken(tokens, index, TokenType::LParen)) return nullptr;

	auto expr = ParseExpression(tokens, index);
	if (!expr) return nullptr;

	if (!RequireToken(tokens, index, TokenType::RParen)) return nullptr;

	return expr;
}

// @refactor: save a symbol table with each block/scope
ref<AST::Sequence> Parser::ParseBlock(const vector<Token>& tokens, int& index) {
	if (!RequireToken(tokens, index, TokenType::LBrace)) return nullptr;

	auto seq = ParseSequence(tokens, index);
	if (!seq) return nullptr;

	if (!RequireToken(tokens, index, TokenType::RBrace)) return nullptr;

	return seq;
}

template<typename T>
inline ref<T> Parser::CreateNode(const Token& tkn)
{
	// @optimize: use big memory block instead
	ref<T> node = RefTo<T>();
	node->filepath = tkn.filepath;
	node->line = tkn.line;
	node->pos = tkn.pos;
	return node;
}
