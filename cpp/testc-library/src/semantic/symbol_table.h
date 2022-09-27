// example:
//
//							0 Symbol Table: global
// proc main(					0.0 Symbol: "main" - proc
//								0.1 Symbol Table: parameters
// ) {								0.1.0 Symbol Table: body
// 	  var x = 10;						0.1.0.0 Symbol: "x" - value
// 	  if(
//       0 < x							value reference -> "x"
//    ) {								0.1.0.1 Symbol Table: if
// 	     print(								proc reference -> "print"
//          x								value reference -> "x"
//       );
//    }
// 	  else {							0.1.0.2 Symbol Table: else
// 	     var y								0.1.0.2.0 Symbol: "y" - value
//          = x;							value reference -> "x"
//    }
// }

#pragma once

#include "basic/include.h"
#include "common/result.h"
#include "util/string_view.h"

namespace testc {

    struct AST_Node;
	
	struct Symbol;
	struct Symbol_Table;

	enum class Symbol_Type {
		Procedure, Value
	};

	// symbols are things like variables, procedures, ...
	// this compiler identifies symbols from the syntax of the source code.
	struct Symbol {
		Symbol_Type type;

		const String_View& identifier;

		// the position within the symbol table.
		// the symbol table can be accessed over the defining node.
		size_t position;

		// the node which defines this symbol..
		Ref<AST_Node> definition;

		// a list of nodes that reference this symbol.
		List<Ref<AST_Node>> references;
	};

	// a table containing symbols. this acts as a kind of context.
    struct Symbol_Table {
		// the parent symbol table containing all symbols also known to this tables symbols.
		Nullable<Ref<Symbol_Table>> parent;
		
		List<Ref<Symbol_Table>> children;

		// the position of this symbol table within its parent.
		size_t position = 0;

		// the position for this tables symbols and child tables.
		size_t counter = 0;

		// all symbols in this symbol list in order.
		Map<Symbol_Type, List<Owner<Symbol>>> symbols;

		// all unresolved procedure calls ie. calls to an unresolved identifier.
        Map<String_View, List<Ref<AST_Node>>> unresolved_proc_calls;

		// all unresolved value usages ie. usages of an unresolved value (eg. variables).
        Map<String_View, List<Ref<AST_Node>>> unresolved_value_usages;
    };

	// searches for this value symbol in this table and all parent tables.
	// #returns:
	//   the symbol with this identifier or nullptr if the tables don't contain such symbol.
	Nullable<Ref<Symbol>> find_value_symbol(Ref<Symbol_Table> table, const String_View& identifier);

	// tries to add a new value symbol to this table.
	// #returns:
	//   all compiler messages that were raised while trying to add the new symbol and
	//   true when the symbol was added or false when a symbol with this identifier was already
	//   defined.
	Result add_value_symbol(Ref<Symbol_Table> table,
		const String_View& identifier, const Ref<AST_Node>& definition);

	// destroys the symbol table and cleans up its symbols
	void destroy(Owner<Symbol_Table> table);

}
