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
		if (!ValidateProcedure(proc)) return false;
	}
	return true;
}

bool Analyzer::ValidateProcedure(ref<Procedure> proc) {
	ASSERT(proc);
	return ValidateSequence(proc->body);
}

bool Analyzer::ValidateSequence(ref<Sequence> seq) {
	ASSERT(seq);
	ASSERT(seq->symbols);

	const map<string, vector<ref<Call>>>& unresolvedCalls = seq->symbols->GetUnresolvedCalls();
	for (const auto& p : unresolvedCalls) {
		for (auto call : p.second) {
			errh.PushErr("procedure '" + call->identifier + "' is not declared");
		}
	}
	if (!unresolvedCalls.empty()) return false;
	
	for (auto stm : seq->statements) {
		if (!ValidateStatement(stm)) return false;
	}
	return true;
}

bool Analyzer::ValidateStatement(ref<Statement> stm) {
	ASSERT(stm);
	switch (stm->GetType()) {
	case Type::Declaration:		return ValidateDeclaration(	CastTo<Declaration>(stm));
	case Type::While:			return ValidateWhile(		CastTo<While>(stm));
	case Type::IfElse:			return ValidateIfElse(		CastTo<IfElse>(stm));
	case Type::Assignment:		return ValidateAssignment(	CastTo<Assignment>(stm));
	case Type::Call:			return ValidateCall(		CastTo<Call>(stm));
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

bool Analyzer::ValidateExpression(ref<Expression> expr) {
	ASSERT(expr);
	switch (expr->GetType()) {
	case Type::BinOp:		return ValidateBinaryOperator(	CastTo<BinaryOperator>(expr));
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

bool Analyzer::ValidateDeclaration(ref<Declaration> decl) {
	ASSERT(decl);
	return ValidateExpression(decl->value);
}

bool Analyzer::ValidateWhile(ref<While> whileStm) {
	ASSERT(whileStm);
	return ValidateExpression(whileStm->condition)
		&& ValidateSequence(whileStm->body);
}

bool Analyzer::ValidateIfElse(ref<IfElse> ifElse) {
	ASSERT(ifElse);
	return ValidateExpression(ifElse->condition)
		&& ValidateSequence(ifElse->ifBody)
		&& (ifElse->elseBody ? ValidateSequence(ifElse->elseBody) : true);
}

bool Analyzer::ValidateAssignment(ref<Assignment> assignm) {
	ASSERT(assignm);
	return ValidateExpression(assignm->value);
}

bool Analyzer::ValidateCall(ref<Call> call) {
	ASSERT(call);
	for (auto arg : call->args) {
		if (!ValidateExpression(arg)) return false;
	}
	return true;
}


bool Analyzer::ValidateBinaryOperator(ref<BinaryOperator> binOp) {
	ASSERT(binOp);
	return ValidateExpression(binOp->lhs)
		&& ValidateExpression(binOp->rhs);
}

bool Analyzer::ValidateConstant(ref<Constant> cnst) {
	ASSERT(cnst);
	return true;
}

bool Analyzer::ValidateVariable(ref<Variable> var) {
	ASSERT(var);
	return true;
}

