#pragma once

#include <testclib.h>

struct Compiler_Options {
	const File source_file;
};

Compiler_Options parse_compiler_options(const int argc, const char** argv);
