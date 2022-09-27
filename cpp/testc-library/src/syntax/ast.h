#pragma once

#include "basic/include.h"
#include "util/string_view.h"
#include "common/location.h"
#include "semantic/operator.h"

namespace testc {

	struct Symbol_Table;

	struct AST_Node;
	struct AST_Procedure;
	struct AST_Statement;
	struct AST_Block;
	struct AST_Declaration;
	struct AST_Assignment;
	struct AST_While;
	struct AST_If_Else;
	struct AST_Call;
	struct AST_Expression;
	struct AST_Constant;
	struct AST_Variable;
	struct AST_Binary_Op;

	enum class AST_Type {
		None			= 0x000,
		Built_In		= 0x100 << 0,
		Procedure		= 0x100 << 1,
		Container		= 0x100 << 2,

		Statement		= 0x100 << 3,
		Block			= Statement | 1,
		Declaration		= Statement | 2,
		Assignment		= Statement | 3,
		While			= Statement | 4,
		If_Else			= Statement | 5,
		Call			= Statement | 6,

		Expression		= 0x100 << 4,
		Constant		= Expression | 1,
		Variable		= Expression | 2,
		Binary_Op		= Expression | 3,
	};

	// Common parent struct for all nodes in the abstract syntax tree (AST).
	struct AST_Node {
		AST_Type type = AST_Type::None;
		Location location;

		// the symbol table containing all symbols of this node and its child nodes
		// when they don't have their own symbol table.
		Shared<Symbol_Table> symbols;
	};

	struct AST_Procedure : public AST_Node {
		String_View identifier;
		Owner<AST_Block> body;
		List<Owner<AST_Declaration>> parameters;
	};

	// Exists to ensure stronger C++ type constraints
	struct AST_Statement : public AST_Node {};

	struct AST_Block : public AST_Statement {
		List<Owner<AST_Statement>> statements;
	};

	struct AST_Declaration : public AST_Statement {
		String_View identifier;
		Nullable<Owner<AST_Expression>> value;
	};

	struct AST_Assignment : public AST_Statement {
		String_View identifier;
		Owner<AST_Expression> value;
	};

	struct AST_While : public AST_Statement {
		Owner<AST_Expression> condition;
		Owner<AST_Block> body;
	};

	struct AST_If_Else : public AST_Statement {
		Owner<AST_Expression> condition;
		Owner<AST_Block> if_body;
		Nullable<Owner<AST_Block>> else_body;
	};

	struct AST_Call : public AST_Statement {
		String_View identifier;
		// @consider: introduce a AST_Arguments node
		List<Owner<AST_Expression>> arguments;
	};

	// Exists to ensure stronger C++ type constraints
	struct AST_Expression : public AST_Node {};

	struct AST_Constant : public AST_Expression {
		short value;
	};

	struct AST_Variable : public AST_Expression {
		String_View identifier;
	};

	struct AST_Binary_Op : public AST_Expression {
		Operator op;
		Owner<AST_Expression> lhs;
		Owner<AST_Expression> rhs;
	};

	const string& to_string(AST_Type type);
	string to_string(const AST_Node& node, byte indentations = 0);

	// destroys the node and clean's up its members.
	void destroy(Owner<AST_Procedure> node);
	void destroy(Owner<AST_Block> node);
	void destroy(Owner<AST_Statement> node);
	void destroy(Owner<AST_Call> node);

}
