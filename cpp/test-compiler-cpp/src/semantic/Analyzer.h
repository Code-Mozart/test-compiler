#pragma once

#include "errh/ErrorHandler.h"
#include "semantic/ast/include.h"
#include "semantic/SymbolTable.h"

class Analyzer {
public:
	Analyzer(ErrorHandler& errh) : errh(errh) {}
	bool ValidateAST(ref<AST::Container> cont);
private:
	bool ValidateProcedure(ref<AST::Procedure> proc, SymbolTable* symbols);
	bool ValidateSequence(ref<AST::Sequence> seq, SymbolTable* symbols);
	bool ValidateStatement(ref<AST::Statement> stm, SymbolTable* symbols);
	bool ValidateExpression(ref<AST::Expression> expr, SymbolTable* symbols);

	bool ValidateDeclaration(ref<AST::Declaration> decl, SymbolTable* symbols);
	bool ValidateWhile(ref<AST::While> whileStm, SymbolTable* symbols);
	bool ValidateIfElse(ref<AST::IfElse> ifElse, SymbolTable* symbols);
	bool ValidateAssignment(ref<AST::Assignment> assignm, SymbolTable* symbols);
	bool ValidateCall(ref<AST::Call> call, SymbolTable* symbols);

	bool ValidateBinaryOperator(ref<AST::BinaryOperator> binOp, SymbolTable* symbols);
	bool ValidateConstant(ref<AST::Constant> cnst);
	bool ValidateVariable(ref<AST::Variable> var);
private:
	ErrorHandler& errh;
};
