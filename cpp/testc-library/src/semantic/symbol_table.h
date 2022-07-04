#pragma once

#include "basic/include.h"
#include "util/string_view.h"

namespace testc {

    struct AST_Node;

	enum class Symbol_Type {
		Procedure, Value
	};

	struct Symbol {
		Symbol_Type type;
		String_View identifier;
		Ref<AST_Node> definition;
	};

    struct Symbol_Table {
        Ref<AST_Node> owner;
        Map<Symbol_Type, List<Owner<Symbol>>> symbols;
        Map<String_View, List<Ref<AST_Node>>> unresolved_usages;
    };

}
