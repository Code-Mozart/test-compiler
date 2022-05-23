#include "Analyzer.h"

#include "Exception.h"

using namespace AST;

bool Analyzer::ValidateAST(ref<Container> cont) {
	ASSERT(cont);
	ASSERT(cont->symbols);
	if (!cont->symbols->GetProc("main")) {
		errh.PushErr("there was no main procedure", *cont);
		return false;
	}
	for (auto proc : cont->procedures) {
		if (!ValidateProcedure(proc, cont->symbols.get())) return false;
	}
	return true;
}

bool Analyzer::ValidateProcedure(ref<Procedure> proc, SymbolTable* symbols) {
	ASSERT(proc);
	ASSERT(symbols);
	for (const auto& param : proc->parameters) {
		if (!ValidateDeclaration(param, symbols));
	}
	return ValidateSequence(proc->body, symbols);
}

bool Analyzer::ValidateSequence(ref<Sequence> seq, SymbolTable* symbols) {
	ASSERT(seq);
	ASSERT(seq->symbols);
	ASSERT(symbols);
	// ASSERT(seq->symbols->pParent == symbols);

	const map<string, vector<ref<Call>>>& unresolvedCalls = seq->symbols->GetUnresolvedCalls();
	for (const auto& p : unresolvedCalls) {
		for (auto call : p.second) {
			errh.PushErr("procedure '" + call->identifier + "' is not declared");
		}
	}
	if (!unresolvedCalls.empty()) return false;
	
	for (auto stm : seq->statements) {
		if (!ValidateStatement(stm, seq->symbols.get())) return false;
	}
	return true;
}

bool Analyzer::ValidateStatement(ref<Statement> stm, SymbolTable* symbols) {
	ASSERT(stm);
	ASSERT(symbols);
	switch (stm->GetType()) {
	case Type::Declaration:		return ValidateDeclaration(	CastTo<Declaration>(stm), symbols);
	case Type::While:			return ValidateWhile(		CastTo<While>(stm), symbols);
	case Type::IfElse:			return ValidateIfElse(		CastTo<IfElse>(stm), symbols);
	case Type::Assignment:		return ValidateAssignment(	CastTo<Assignment>(stm), symbols);
	case Type::Call:			return ValidateCall(		CastTo<Call>(stm), symbols);
	default: {
		if (stm->IsType(Type::Statement)) {
			throw IncorrectImplException(__FILE__, __LINE__,
				string() + "unhandled statement type " + ToString(stm->GetType()));
		}
		else {
			throw IncorrectImplException(__FILE__, __LINE__,
				"ValidateStatement() called with non-statement type");
		}
	}
	}
}

bool Analyzer::ValidateExpression(ref<Expression> expr, SymbolTable* symbols) {
	ASSERT(expr);
	ASSERT(symbols);
	switch (expr->GetType()) {
	case Type::BinOp:		return ValidateBinaryOperator(	CastTo<BinaryOperator>(expr), symbols);
	case Type::Const:		return ValidateConstant(		CastTo<Constant>(expr));
	case Type::Var:			return ValidateVariable(		CastTo<Variable>(expr));
	default: {
		if (expr->IsType(Type::Expression)) {
			throw IncorrectImplException(__FILE__, __LINE__,
				string() + "unhandled expression type " + ToString(expr->GetType()));
		}
		else {
			throw IncorrectImplException(__FILE__, __LINE__,
				"ValidateExpression() called with non-expression type");
		}
	}
	}
}

bool Analyzer::ValidateDeclaration(ref<Declaration> decl, SymbolTable* symbols) {
	ASSERT(decl);
	ASSERT(symbols);
	return decl->value ? ValidateExpression(decl->value, symbols) : true;
}

bool Analyzer::ValidateWhile(ref<While> whileStm, SymbolTable* symbols) {
	ASSERT(whileStm);
	ASSERT(symbols);
	return ValidateExpression(whileStm->condition, symbols)
		&& ValidateSequence(whileStm->body, symbols);
}

bool Analyzer::ValidateIfElse(ref<IfElse> ifElse, SymbolTable* symbols) {
	ASSERT(ifElse);
	ASSERT(symbols);
	return ValidateExpression(ifElse->condition, symbols)
		&& ValidateSequence(ifElse->ifBody, symbols)
		&& (ifElse->elseBody ? ValidateSequence(ifElse->elseBody, symbols) : true);
}

bool Analyzer::ValidateAssignment(ref<Assignment> assignm, SymbolTable* symbols) {
	ASSERT(assignm);
	ASSERT(symbols);
	return ValidateExpression(assignm->value, symbols);
}

bool Analyzer::ValidateCall(ref<Call> call, SymbolTable* symbols) {
	ASSERT(call);
	ASSERT(symbols);

	auto proc = symbols->GetProc(call->identifier);
	// unresolved symbol should be handled already
	ASSERT(proc);
	size_t nArgs = call->args.size();
	size_t nParams = proc->parameters.size();
	if (nArgs < nParams) {
		errh.PushErr("procedure '" + proc->identifier + "' has " + std::to_string(nParams)
			+ " parameters, but only " + std::to_string(nArgs) + " arguments are passed", *call);
		return false;
	}
	else if (nArgs > nParams) {
		errh.PushErr("procedure '" + proc->identifier + "' has only " + std::to_string(nParams)
			+ " parameters, but " + std::to_string(nArgs) + " arguments are passed", *call);
		return false;
	}

	for (auto arg : call->args) {
		if (!ValidateExpression(arg, symbols)) return false;
	}

	return true;
}


bool Analyzer::ValidateBinaryOperator(ref<BinaryOperator> binOp, SymbolTable* symbols) {
	ASSERT(binOp);
	ASSERT(symbols);
	return ValidateExpression(binOp->lhs, symbols)
		&& ValidateExpression(binOp->rhs, symbols);
}

bool Analyzer::ValidateConstant(ref<Constant> cnst) {
	ASSERT(cnst);
	return true;
}

bool Analyzer::ValidateVariable(ref<Variable> var) {
	ASSERT(var);
	return true;
}

