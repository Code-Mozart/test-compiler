#include <testclib.h>
#include "compiler_options.h"

using namespace testc;

void print_exception(const std::exception& e) {
	printf("%s\n", (string("[EXCEPTION] ") + e.what()).c_str());
}

void print_exception(const string& msg) {
	printf("%s\n", (string("[EXCEPTION] ") + msg).c_str());
}

void print_message(const Compiler_Message& msg) {
	printf("%s\n", as_string(msg).c_str());
}

void print_messages(const List<Ref<const Compiler_Message>>& messages) {
	for (const Ref<const Compiler_Message>& msg : messages) {
		print_message(*msg);
	}
}

void print_mem_stats() {
	const Memory_Metrics& mm = get_memory_metrics();

	printf("-- MEMORY METRICS ----------------------\n");
	printf("allocations         %zu\n", mm.total_allocations);
	printf("deallocations       %zu\n", mm.total_deallocations);
	printf("system allocations  %zu\n",mm.system_allocations);
	printf("                    %.2f%% of all allocs\n",
		((float)mm.system_allocations / (float)mm.total_allocations * 100.0f));
	printf("system memory       %zu bytes\n", mm.total_system_memory);
	printf("memory used         %zu bytes\n", mm.total_memory_used);
	printf("                    %.2f%%\n",
		((float)mm.total_memory_used / (float)mm.total_system_memory * 100.0f));
	printf("----------------------------------------\n");
}

void on_exit() {
	print_mem_stats();
}

int main(const int argc, const char** argv) {
	try {
		Compiler_Options options = parse_compiler_options(argc, argv);

		File_Read_Result read_result = read_from_file(&options.source_file);
		print_messages(read_result.messages);
		if (has_failed(read_result)) {
			on_exit();
			return 1;
		}

		Attach_Lexer_Result attach_result = attach_lexer_on(read_result.src_info);
		print_messages(attach_result.messages);
		if (has_failed(attach_result)) {
			on_exit();
			return 1;
		}

		//temp
		for (const auto& token : attach_result.lexer->tokens) {
			printf("%s\n", to_string(*token).c_str());
		}

		Owner<Symbol_Table> global_context = create_symbol_table();
		Parser_Result_Multi parser_result = parse_procedures(attach_result.lexer, global_context);
		print_messages(parser_result.messages);
		if (has_failed(parser_result)) {
			on_exit();
			return 1;
		}

		Result resolve_references_result = resolve_references(parser_result.nodes, global_context);
		print_messages(resolve_references_result.messages);
		if (has_failed(resolve_references_result)) {
			on_exit();
			return 1;
		}
		for (Ref<AST_Procedure> proc : parser_result.nodes) {
			Result type_check_result = type_check(proc);
			print_messages(type_check_result.messages);
			if (has_failed(type_check_result)) {
				on_exit();
				return 1;
			}
		}

		Generator_Result generator_result = generate_instructions(parser_result.nodes, global_context);
		print_messages(generator_result.messages);
		if (has_failed(generator_result)) {
			on_exit();
			return 1;
		}
		Result check_resolved_result = check_nothing_unresolved(generator_result);
		print_messages(check_resolved_result.messages);
		if (has_failed(check_resolved_result)) {
			on_exit();
			return 1;
		}
		
		string instructions_string = write_instructions(generator_result.instructions);
		File instructions_file = File(options.source_file).replace_extension("vmc");
		File_Write_Result write_result = write_to_file(&instructions_file, instructions_string);
		print_messages(write_result.messages);
		if (has_failed(write_result)) {
			on_exit();
			return 1;
		}
		printf("Successfully compiled '%s' to '%s'\n", options.source_file.string().c_str(), instructions_file.string().c_str());

		//  @missing: cleanup/relasing resources
	}
	catch (const Exception& e) {
		print_exception(e);
	}
	catch (const std::exception& e) {
		print_exception(e);
	}
	catch (const char* e) {
		print_exception(e);
	}
	catch (...) {
		print_exception("unknown error occured");
	}

	on_exit();
	return 0;
}
