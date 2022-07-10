#include "compiler_options.h"
#include <filesystem>

namespace fs = std::filesystem;
using namespace testc;

Compiler_Options parse_compiler_options(const int argc, const char** argv) {
	File file;
	bool file_set = false;
	for (int i = 0; i < argc; i++) {
		const string arg = argv[i];
		ASSERT(!arg.empty());
		if (arg[0] == '-') {
			NOT_IMPL();
		}
		else {
			file = File(arg);
			if (!fs::exists(file)) {
				throw Invalid_Argument_Exception("Source file '" + file.string() + "' does not exist");
			}
			else if (!fs::is_regular_file(file)) {
				throw Invalid_Argument_Exception("Source file '" + file.string() + "' is not a valid file");
			}
			file_set = true;
		}
	}

	if (!file_set) {
		throw Invalid_Argument_Exception("No source file was passed as an argument");
	}
	return Compiler_Options{ file };
}
