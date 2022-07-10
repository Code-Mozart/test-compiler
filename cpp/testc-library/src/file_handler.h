#pragma once

#include "basic/include.h"
#include "common/result.h"
#include "common/compiler_message.h"
#include "source_info.h"

namespace testc {

	// The result of a file read.
	// As a file read may fail it is not guaranteed that the values are valid.
	struct File_Read_Result {
		// Indicates whether the file read was successful in which case the
		// values are valid.
		bool success = false;

		List<Owner<const Compiler_Message>> messages;

		// A source info object to the file.
		const Source_Info src_info;

		// The actual content buffer.
		const string content;
	};

	// #returns:
	//   whether reading the file resulted in a failure or not.
	inline bool has_failed(const File_Read_Result& result) { return !result.success; }

	// The result of a file write.
	using File_Write_Result = Result;

	// Opens the specified file and reads its content.
	File_Read_Result read_from_file(const Ref<const File> file);

	// Creates the specified file and writes the content.
	// The file will be overridden if it already exists.
	File_Write_Result write_to_file(const Ref<const File> file, const string& content);

	struct File_Access_Exception : public Exception {
	public:
		File_Access_Exception(const Ref<const File> file)
			: Exception(STRINGIFY(File_Access_Exception), file->string()) {}
		File_Access_Exception(const Ref<const File> file, const string& msg)
			: Exception(STRINGIFY(File_Access_Exception), file->string() + ": " + msg) {}
	};

}
