#include "symbol_table.h"
#include "basic/memory.h"
#include "syntax/ast.h"

namespace testc {

	Nullable<Ref<Symbol>> find_value_symbol(Ref<Symbol_Table> table, const String_View& identifier) {
        for (const Ref<Symbol>& sym : table->symbols[Symbol_Type::Value]) {
            if (sym->identifier == identifier) {
                return sym;
            }
        }
        if (table->parent) {
            return find_value_symbol(table->parent, identifier);
        }
        else {
            return nullptr;
        }
    }

	Result add_value_symbol(Ref<Symbol_Table> table,
		    const String_View& identifier, const Ref<AST_Node>& definition) {
        Nullable<Ref<Symbol>> existing_symbol = find_value_symbol(table, identifier);
        if (existing_symbol) {
            return Result{
                false,
                { raise_error("value symbol '" + identifier + "' is already defined at "
                    + as_string(existing_symbol->definition->location), definition->location) }
            };
        }
        else {
            List<Ref<Symbol>>& value_symbols = table->symbols[Symbol_Type::Value];
            value_symbols.emplace_back(new (allocate<Symbol>()) Symbol{
                Symbol_Type::Value,
                identifier,
                table->counter++,
                definition
            });
            return Result{ true, {} };
        }
    }

    void destroy(Owner<Symbol_Table> table) {
        NOT_IMPL();
    }

}
