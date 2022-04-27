#pragma once

#include "util/Util.h"

enum class Operation {
	Stop,
	Push, Pop, Load, Store,
	Jump, JumpIf0, JumpNot0,
	Add, Subtract, Multiply, Divide, Modulo,
	LessThan,
	ConsoleOut
};

inline static string ToString(Operation op) {
	switch (op) {
	case Operation::Stop:			return "stop";

	case Operation::Push:			return "push";
	case Operation::Pop:			return "pop";
	case Operation::Load:			return "load";
	case Operation::Store:			return "str";

	case Operation::Jump:			return "jmp";
	case Operation::JumpIf0:		return "jz";
	case Operation::JumpNot0:		return "jnz";

	case Operation::Add:			return "add";
	case Operation::Subtract:		return "sub";
	case Operation::Multiply:		return "mul";
	case Operation::Divide:			return "div";
	case Operation::Modulo:			return "mod";

	case Operation::LessThan:		return "lt";

	case Operation::ConsoleOut:		return "cout";
	}
	return "error";
}

inline static bool HasParam(Operation op) {
	switch (op) {
	case Operation::Push:	
	case Operation::Load:	
	case Operation::Store:	
	case Operation::Jump:	
	case Operation::JumpIf0:
	case Operation::JumpNot0:
		return true;
	default:
		return false;
	}
}

struct Instruction {
	Operation op;
	short arg = 0;
};
