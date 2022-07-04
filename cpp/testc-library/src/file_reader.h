#pragma once

#include "basic/include.h"
#include "source_info.h"

namespace testc {

	// @improve @refactor: generate compiler messages instead of throwing an exception
	// @explain: exceptions should only be used, if the compiler itself encounters an
	//           unexpected situation

	// The result of a successfull file read operation.
	// Holds a source info and a string containing the actual
	// content.
	struct File_Reader_Result {
		const Source_Info src_info;
		const string content;
	};

	// Opens the specified file and reads its content.
	File_Reader_Result read_from_file(const Ref<const File> file);

	struct File_Access_Exception : public Exception {
	public:
		File_Access_Exception(const Ref<const File> file)
			: Exception(STRINGIFY(File_Access_Exception), file->string()) {}
		File_Access_Exception(const Ref<const File> file, const string& msg)
			: Exception(STRINGIFY(File_Access_Exception), file->string() + ": " + msg) {}
	};

}
