#include "Generator.h"

#include "generator/Instruction.h"

using namespace AST;

string CodeGenerator::Generate(ref<Node> root) {
	vector<Instruction> instructions;
	RuntimeStack stack;
	Generate(root, instructions, stack);
	instructions.emplace_back(Instruction{ Operation::Stop });

	string bytecode;
	for (const auto& instr : instructions) {
		bytecode += ToString(instr.op);
		if (HasParam(instr.op))
			bytecode += " " + std::to_string(instr.arg);
		bytecode += "\n";
	}

	return bytecode;
}

vector<Instruction>& CodeGenerator::Generate(ref<Node> node, vector<Instruction>& instructions, RuntimeStack& stack) {
	if (!node) throw NullptrException("node is null");
	
	switch (node->GetType())
	{
	case Type::Sequence: {
		const auto& seq = (Sequence&)*node;
		stack.EnterScope(node);
		for (const auto& stm : seq.statements) {
			Generate(stm, instructions, stack);
		}
		stack.ExitScope();
		break;
	}


	case Type::Declaration: {
		const auto& decl = (Declaration&)*node;
		Generate(decl.value, instructions, stack);
		stack.PutVar(decl.identifier);
		break;
	}
	case Type::Assignment: {
		const auto& assignm = (Assignment&)*node;
		Generate(assignm.value, instructions, stack);
		short offset = stack.GetOffset(assignm.variable);
		instructions.emplace_back(Instruction{ Operation::Store, offset });
		stack.Shrink();
		break;
	}
	case Type::While: {
		const auto& whileStm = (While&)*node;
		short count = instructions.size();
		Generate(whileStm.condition, instructions, stack);

		vector<Instruction> bodyInstructions;
		// shrink stack as if the condition result had been processed already
		stack.Shrink();
		stack.EnterScope(node);
		// @refactor: replace with Generate(whileStm.body)
		for (const auto& stm : whileStm.body->statements) {
			Generate(stm, bodyInstructions, stack);
		}
		byte popCount = stack.ExitScope();
		for (byte i = 0; i < popCount; i++)
			bodyInstructions.emplace_back(Instruction{ Operation::Pop });
		// now revert it to its original value
		stack.Grow();
		// writing the instr JZ to jump over the while body if the condition is NOT met
		instructions.emplace_back(Instruction{ Operation::JumpIf0, (short)(bodyInstructions.size() + 2) });
		stack.Shrink();
		for (const auto& instr : bodyInstructions)
			instructions.emplace_back(instr);
		instructions.emplace_back(Instruction{ Operation::Jump, (short)(count - instructions.size()) });

		break;
	}
	case Type::IfElse: {
		// @missing: implement else body

		const auto& ifStm = (IfElse&)*node;
		Generate(ifStm.condition, instructions, stack);

		vector<Instruction> ifBodyInstructions;
		// shrink stack as if the condition result had been processed already
		stack.Shrink();
		stack.EnterScope(node);
		// @refactor: replace with Generate(ifStm.ifBody)
		for (const auto& stm : ifStm.ifBody->statements) {
			Generate(stm, ifBodyInstructions, stack);
		}
		byte popCount = stack.ExitScope();
		for (byte i = 0; i < popCount; i++)
			ifBodyInstructions.emplace_back(Instruction{ Operation::Pop });
		// now revert it to its original value
		stack.Grow();
		// writing the instr JZ to jump over the if body if the condition is NOT met
		// additionally jump over the JMP instruction at the end of the if body
		instructions.emplace_back(Instruction{
			Operation::JumpIf0,
			(short)(ifBodyInstructions.size() + 1 + (ifStm.elseBody != nullptr))
			});
		stack.Shrink();
		for (const auto& instr : ifBodyInstructions)
			instructions.emplace_back(instr);

		// @later: replace with null check to scope/sequence
		if (ifStm.elseBody) {
			vector<Instruction> elseBodyInstructions;
			stack.EnterScope(node);
			// @refactor: replace with Generate(ifStm.elseBody)
			for (const auto& stm : ifStm.elseBody->statements) {
				Generate(stm, elseBodyInstructions, stack);
			}
			byte popCount = stack.ExitScope();
			for (byte i = 0; i < popCount; i++)
				elseBodyInstructions.emplace_back(Instruction{ Operation::Pop });

			// writing the instr JMP to jump over the else body from the end of the if body
			instructions.emplace_back(Instruction{ Operation::Jump, (short)(elseBodyInstructions.size() + 1) });

			for (const auto& instr : elseBodyInstructions)
				instructions.emplace_back(instr);
		}

		break;
	}
	case Type::Call: {
		const auto& call = (Call&)*node;
		for (const auto& arg : call.args) {
			Generate(arg, instructions, stack);
		}
		if (call.identifier == "print") {
			instructions.emplace_back(Instruction{ Operation::ConsoleOut });
			stack.Shrink();
		}
		else {
			throw NotImplementedException(__FILE__, __LINE__, "only print calls are supported for now");
		}
		break;
	}


	case Type::Const: {
		const auto& cnst = (Constant&)*node;
		instructions.emplace_back(Instruction{ Operation::Push, cnst.value });
		stack.Grow();
		break;
	}
	case Type::Var: {
		const auto& var = (Variable&)*node;
		short offset = stack.GetOffset(var.identifier);
		instructions.emplace_back(Instruction{ Operation::Load, offset });
		stack.Grow();
		break;
	}
	case Type::BinOp: {
		const auto& binOp = (BinaryOperator&)*node;
		Generate(binOp.lhs, instructions, stack);
		Generate(binOp.rhs, instructions, stack);

		Operation op;
		switch (binOp.op) {
		case ASTOperator::LessThan:		op = Operation::LessThan;	break;

		case ASTOperator::Plus:			op = Operation::Add;		break;
		case ASTOperator::Minus:		op = Operation::Subtract;	break;
		case ASTOperator::Asterisk:		op = Operation::Multiply;	break;
		case ASTOperator::Slash:		op = Operation::Divide;		break;
		
		default: {
			throw NotImplementedException(__FILE__, __LINE__,
				string() + "operator '" + (char)binOp.op + "' not supported by code generator");
		}
		}

		instructions.emplace_back(Instruction{ op });
		stack.Shrink();

		break;
	}


	default: {
		throw IncorrectImplException(__FILE__, __LINE__,
			"type '" + ToString(node->GetType()) + "' not supported by code generator");
	}
	}

	return instructions;
}
