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


void Parser::PushErr(const string& text, const Token& tkn) {
	errh.PushErr(text, tkn);
}


ref<Node> Parser::BuildAST(const vector<Token>& tokens)
{
	ref<Sequence> root = RefTo<Sequence>();
	root->filepath = "filepath";
	root->line = 0;
	root->pos = 0;
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

// @refactor: remove identifier shadowing
ref<Statement> Parser::ParseStatement(const vector<Token>& tokens, int& index)
{
	const Token& t = tokens[index++];
	switch (t.type)
	{
	case TokenType::Identifier: {
		switch (ParseKeyword(t.text)) {
		case Keyword::VarDecl: {
			// declarations are at least 5 tokens
			// @fix: if the expression is more than 1 token, this doesnt ensure that index++ will not throw
			if (index + 3 >= tokens.size()) {
				PushErr("incomplete declaration", t);
				return nullptr;
			}

			const Token& identTkn = tokens[index++];
			if (identTkn.type != TokenType::Identifier) {
				PushErr("expected an identifier", identTkn);
				return nullptr;
			}

			const Token& opTkn = tokens[index++];
			if (opTkn.type != TokenType::Operator || opTkn.text != "=") {
				PushErr("expected a '='", opTkn);
				return nullptr;
			}

			auto val = ParseExpression(tokens, index);
			if (!val) return nullptr;

			const Token& semTkn = tokens[index++];
			if (semTkn.type != TokenType::Semicolon) {
				PushErr("expected a ';'", semTkn);
				return nullptr;
			}

			ref<Declaration> decl = CreateNode<Declaration>(t);
			decl->identifier = identTkn.text;
			decl->value = val;
			return decl;
		}
		case Keyword::WhileStm: {
			// while stm are at least 5 tokens
			// @fix: if the expression is more than 1 token, this doesnt ensure that index++ will not throw
			if (index + 3 >= tokens.size()) {
				PushErr("incomplete while statement", t);
				return nullptr;
			}

			const Token& lparenTkn = tokens[index];
			if (lparenTkn.type != TokenType::LParen) {
				PushErr("expected a '('", lparenTkn);
				return nullptr;
			}

			const Token& condTkn = tokens[index + 1];
			auto cond = ParseExpression(tokens, index);
			if (!cond) return nullptr;
			if (!cond->IsType(Type::Expression)) {
				PushErr("expected an expression", condTkn);
				return nullptr;
			}
			
			const Token& lbraceTkn = tokens[index++];
			if (lbraceTkn.type != TokenType::LBrace) {
				PushErr("expected a '{'", lbraceTkn);
				return nullptr;
			}

			auto whileStm = CreateNode<While>(t);
			whileStm->condition = cond;

			// @refactor: replace with call to ParseSequence()
			whileStm->body = CreateNode<Sequence>(lbraceTkn);
			while (tokens[index].type != TokenType::RBrace) {
				if (index + 1 >= tokens.size()) {
					PushErr("unexpected EOF, expected a '}'", tokens.back());
					return nullptr;
				}

				auto stm = ParseStatement(tokens, index);
				if (!stm) return nullptr;
				whileStm->body->statements.emplace_back(stm);
			}
			// consume rbrace
			index++;

			return whileStm;
		}
		case Keyword::IfStm: {
			// if stm are at least 5 tokens
			// @fix: if the expression is more than 1 token, this doesnt ensure that index++ will not throw
			if (index + 3 >= tokens.size()) {
				PushErr("incomplete if statement", t);
				return nullptr;
			}

			const Token& lparenTkn = tokens[index];
			if (lparenTkn.type != TokenType::LParen) {
				PushErr("expected a '('", lparenTkn);
				return nullptr;
			}

			const Token& condTkn = tokens[index + 1];
			auto cond = ParseExpression(tokens, index);
			if (!cond) return nullptr;
			if (!cond->IsType(Type::Expression)) {
				PushErr("expected an expression", condTkn);
				return nullptr;
			}

			const Token& lbraceTkn = tokens[index++];
			if (lbraceTkn.type != TokenType::LBrace) {
				PushErr("expected a '{'", lbraceTkn);
				return nullptr;
			}

			auto ifStm = CreateNode<IfElse>(t);
			ifStm->condition = cond;

			// @refactor: replace with call to ParseSequence()
			ifStm->ifBody = CreateNode<Sequence>(lbraceTkn);
			while (tokens[index].type != TokenType::RBrace) {
				if (index + 1 >= tokens.size()) {
					PushErr("unexpected EOF, expected a '}'", tokens.back());
					return nullptr;
				}

				auto stm = ParseStatement(tokens, index);
				if (!stm) return nullptr;
				ifStm->ifBody->statements.emplace_back(stm);
			}
			// consume rbrace
			index++;


			// check for else stm
			if (index + 1 < tokens.size()) {
				const Token& candidateTkn = tokens[index];
				if (candidateTkn.type != TokenType::Identifier)
					return ifStm;
				if (ParseKeyword(candidateTkn.text) != Keyword::ElseStm)
					return ifStm;
				// candidateTkn matched "else"
				index++;

				const Token& lbraceTkn = tokens[index++];
				if (lbraceTkn.type != TokenType::LBrace) {
					PushErr("expected a '{'", lbraceTkn);
					return nullptr;
				}

				// @refactor: replace with call to ParseSequence()
				ifStm->elseBody = CreateNode<Sequence>(lbraceTkn);
				while (tokens[index].type != TokenType::RBrace) {
					if (index + 1 >= tokens.size()) {
						PushErr("unexpected EOF, expected a '}'", tokens.back());
						return nullptr;
					}

					auto stm = ParseStatement(tokens, index);
					if (!stm) return nullptr;
					ifStm->elseBody->statements.emplace_back(stm);
				}
				// consume rbrace
				index++;
			}

			return ifStm;
		}
		default: {
			// no keyword -> assignment/call
			const Token& nextTkn = tokens[index++];
			if (nextTkn.type == TokenType::LParen) {
				// function call
				// @improve: allow functions with multiple args (handle parantheses missmatch, add ',' separator)
				
				auto arg1 = ParseExpression(tokens, index);
				if (!arg1) return nullptr;

				auto call = CreateNode<Call>(t);
				call->identifier = t.text;
				call->args.emplace_back(arg1);

				const Token& rparenTkn = tokens[index++];
				if (rparenTkn.type != TokenType::RParen) {
					PushErr("expected a ')'", rparenTkn);
					return nullptr;
				}

				const Token& semTkn = tokens[index++];
				if (semTkn.type != TokenType::Semicolon) {
					PushErr("expected a ';'", semTkn);
					return nullptr;
				}

				return call;
			}
			else if (nextTkn.type == TokenType::Operator) {
				// assignment

				if (nextTkn.text != "=") {
					PushErr("expected an '='", nextTkn);
					return nullptr;
				}

				auto val = ParseExpression(tokens, index);
				if (!val) return nullptr;

				const Token& semTkn = tokens[index++];
				if (semTkn.type != TokenType::Semicolon) {
					PushErr("expected a ';'", semTkn);
					return nullptr;
				}

				auto assignment = CreateNode<Assignment>(t);
				assignment->variable = t.text;
				assignment->value = val;
				return assignment;
			}
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

ref<AST::Expression> Parser::ParseExpression(const vector<Token>& tokens, int& index, byte minPrec) {
	// @improve: handle paranthesis
	auto lhs = ParsePrimary(tokens, index);
	if (!lhs) return nullptr;
	const Token* next = &(tokens[index]);
	while (next->type == TokenType::Operator) {
		const ASTOperator op = (ASTOperator)next->text[0];
		const byte prec = GetOpPrecedence((char)op);
		if (prec > minPrec) {
			index++;
			auto rhs = ParseExpression(tokens, index, prec);

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

//ref<Expression> Parser::BuildExpression(vector<const Token*>& outQueue)
//{
//	const Token& t = *outQueue.back();
//	outQueue.pop_back();
//
//	switch (t.type) {
//	case TokenType::Literal: {
//		auto cnst = CreateNode<Constant>(t);
//		cnst->value = t.value;
//		return cnst;
//	}
//	case TokenType::Identifier: {
//		auto var = CreateNode<Variable>(t);
//		var->identifier = t.text;
//		return var;
//	}
//	case TokenType::Operator: {
//		if (outQueue.size() < 2) {
//			PushErr("expected an expression", t);
//			return nullptr;
//		}
//		auto binOp = CreateNode<BinaryOperator>(t);
//		binOp->op = (ASTOperator)t.text[0];
//		auto rhs = BuildExpression(outQueue);
//		if (!rhs) return nullptr;
//		binOp->rhs = rhs;
//		auto lhs = BuildExpression(outQueue);
//		if (!lhs) return nullptr;
//		binOp->lhs = lhs;
//		return binOp;
//	}
//	}
//	PushErr("illegal token in expression", t);
//	return nullptr;
//}

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
