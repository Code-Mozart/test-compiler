#include <testclib.h>
#include "compiler_options.h"

using namespace testc;

void print_exception(const std::exception& e) {
	printf((string("[EXCEPTION] ") + e.what()).c_str());
}

void print_exception(const string& msg) {
	printf((string("[EXCEPTION] ") + msg).c_str());
}

void print_message(const Compiler_Message& msg) {
	printf("%s\n", as_string(msg).c_str());
}

void print_messages(const List<Ref<const Compiler_Message>>& messages) {
	for (const Ref<const Compiler_Message>& msg : messages) {
		print_message(*msg);
	}
}

int main(const int argc, const char** argv) {
	try {
		Compiler_Options options = parse_compiler_options(argc, argv);

		File_Read_Result read_result = read_from_file(&options.source_file);
		print_messages(read_result.messages);
		if (has_failed(read_result)) {
			return;
		}

		Attach_Lexer_Result attach_result = attach_lexer_on(read_result.src_info);
		print_messages(attach_result.messages);
		if (has_failed(attach_result)) {
			return;
		}

		Parser_Result_Multi parser_result = parse_procedures(attach_result.lexer, nullptr);
		print_messages(parser_result.messages);
		if (has_failed(parser_result)) {
			return;
		}

		Generator_Result generator_result = generate_instructions(parser_result.nodes);
		print_messages(generator_result.messages);
		if (has_failed(generator_result)) {
			return;
		}
		Result check_resolved_result = check_nothing_unresolved(generator_result);
		print_messages(check_resolved_result.messages);
		if (has_failed(check_resolved_result)) {
			return;
		}
		
		string instructions_string = write_instructions(generator_result.instructions);
		File instructions_file = File(options.source_file).replace_extension("vmc");
		File_Write_Result write_result = write_to_file(&instructions_file);
		print_messages(write_result.messages);
		if (has_failed(write_result)) {
			return;
		}
		printf("Successfully compiled '%s' to '%s'", options.source_file.c_str(), instructions_file.c_str());
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
}
