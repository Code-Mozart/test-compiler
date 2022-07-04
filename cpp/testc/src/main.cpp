#include <testclib.h>
#include "compiler_options.h"

using namespace testc;

void print_exception(const std::exception& e) {
	printf((string("[EXCEPTION] ") + e.what()).c_str());
}

void print_exception(const string& msg) {
	printf((string("[EXCEPTION] ") + msg).c_str());
}

int main(const int argc, const char** argv) {
	try {
		Compiler_Options options = parse_compiler_options(argc, argv);
		File_Reader_Result file_content = read_from_file(&options.source_file);

		Owner<Lexer> lexer = attach_lexer_on(file_content.src_info);
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
