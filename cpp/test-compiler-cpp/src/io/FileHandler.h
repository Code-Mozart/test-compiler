#pragma once

#include "Exception.h"
#include "FileInfo.h"

struct FileAccessException : public Exception {
public:
	FileAccessException(const string& path, const string& msg = "")
		: Exception("failed to open file '" + path + "' " + msg) {}
};

namespace FileHandler {

	FileInfo Read(const string& path);
	void Write(const string& content, const string& path);
	string RemoveExtension(const string& filename);

}
