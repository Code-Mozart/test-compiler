#pragma once

#include "errh/ErrorHandler.h"
#include "semantic/ast/include.h"
#include "semantic/SymbolTable.h"

class Analyzer {
public:
	Analyzer(ErrorHandler& errh) : errh(errh) {}
	bool ValidateAST(ref<AST::Container> cont);
private:
	bool ValidateProcedure(ref<AST::Procedure> proc);
	bool ValidateSequence(ref<AST::Sequence> seq);
	bool ValidateStatement(ref<AST::Statement> stm);
	bool ValidateExpression(ref<AST::Expression> expr);

	bool ValidateDeclaration(ref<AST::Declaration> decl);
	bool ValidateWhile(ref<AST::While> whileStm);
	bool ValidateIfElse(ref<AST::IfElse> ifElse);
	bool ValidateAssignment(ref<AST::Assignment> assignm);
	bool ValidateCall(ref<AST::Call> call);

	bool ValidateBinaryOperator(ref<AST::BinaryOperator> binOp);
	bool ValidateConstant(ref<AST::Constant> cnst);
	bool ValidateVariable(ref<AST::Variable> var);
private:
	ErrorHandler& errh;
};
