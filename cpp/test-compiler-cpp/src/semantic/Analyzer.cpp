#include "Analyzer.h"

#include "Exception.h"

using namespace AST;

bool Analyzer::ValidateAST(ref<Node> node, SymbolTable* pSymbols) {
	if (!node) throw NullptrException("ValidateAST() called with null");
	if (!pSymbols && node->GetType() != Type::Sequence)
		throw NullptrException("SymbolTable is null");

	switch (node->GetType()) {
	case Type::Sequence: {
		const auto& seq = (Sequence&)*node;

		// create a new sub-table for symbols => scope
		SymbolTable symbols = SymbolTable(pSymbols);
		for (const auto& stm : seq.statements) {
			if (!ValidateAST(stm, &symbols))
				return false;
		}

		return true;
	}



	case Type::Declaration: {
		const auto& decl = (Declaration&)*node;

		// comes first because the symbol is not yet declared
		if (!ValidateAST(decl.value, pSymbols))
			return false;

		// symbol must be currently unknown
		if (!pSymbols->Add(decl.identifier, node)) {
			const auto& conflictingDef = pSymbols->Find(decl.identifier);
			errh.PushErr("symbol '" + decl.identifier + "' conflicts with " + conflictingDef->Node::ToString(), decl);
			return false;
		}

		return true;
	}
	case Type::Assignment: {
		const auto& assignm = (Assignment&)*node;

		if (!pSymbols->Find(assignm.variable)) {
			errh.PushErr("symbol '" + assignm.variable + "' not found", assignm);
			return false;
		}

		if (!ValidateAST(assignm.value, pSymbols))
			return false;

		return true;
	}
	case Type::While: {
		const auto& whileStm = (While&)*node;

		if (!ValidateAST(whileStm.condition, pSymbols))
			return false;

		SymbolTable subTable = SymbolTable(pSymbols);
		// @refactor: replace with ValidateAST(whileStm.body)
		for (const auto& stm : whileStm.body->statements) {
			if (!ValidateAST(stm, &subTable))
				return false;
		}

		return true;
	}
	case Type::IfElse: {
		const auto& ifStm = (IfElse&)*node;

		if (!ValidateAST(ifStm.condition, pSymbols))
			return false;

		SymbolTable subTableIf = SymbolTable(pSymbols);
		// @refactor: replace with ValidateAST(ifStm.ifBody)
		for (const auto& stm : ifStm.ifBody->statements) {
			if (!ValidateAST(stm, &subTableIf))
				return false;
		}

		if (ifStm.elseBody) {
			SymbolTable subTableElse = SymbolTable(pSymbols);
			// @refactor: replace with ValidateAST(ifStm.elseBody)
			for (const auto& stm : ifStm.elseBody->statements) {
				if (!ValidateAST(stm, &subTableElse))
					return false;
			}
		}

		return true;
	}
	case Type::Call: {
		const auto& call = (Call&)*node;

		if (!pSymbols->Find(call.identifier)) {
			errh.PushErr("symbol '" + call.identifier + "' not found", call);
			return false;
		}

		for (const auto& expr : call.args) {
			if (!ValidateAST(expr, pSymbols))
				return false;
		}

		return true;
	}



	case Type::Const: {
		return true;
	}
	case Type::Var: {
		const auto& var = (Variable&)*node;

		if (!pSymbols->Find(var.identifier)) {
			errh.PushErr("symbol '" + var.identifier + "' not found", var);
			return false;
		}

		return true;
	}
	case Type::BinOp: {
		const auto& binOp = (BinaryOperator&)*node;

		if (!ValidateAST(binOp.lhs, pSymbols) || !ValidateAST(binOp.rhs, pSymbols))
			return false;

		return true;
	}
	default: {
		throw NotImplementedException(__FILE__, __LINE__, "type '" +
			AST::ToString(node->GetType()) +"' not covered in ValidateAST()");
	}
	}
}
