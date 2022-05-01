#include "ASTType.h"

#include "Exception.h"

namespace AST {
	
	string ToString(Type t)
	{
		switch (t)
		{
		case Type::None:		return "None";
		case Type::Sequence:	return "Sequence";
		case Type::BuiltIn:		return "BuiltIn";

		case Type::Declaration:	return "Declaration";
		case Type::Assignment:	return "Assignment";
		case Type::While:		return "While";
		case Type::IfElse:		return "IfElse";
		case Type::Call:		return "Call";

		case Type::Const:		return "Const";
		case Type::BinOp:		return "BinOp";
		case Type::Var:			return "Var";
		}
		throw IncorrectImplException(__FILE__, __LINE__, "enum case not handled");
	}

}
