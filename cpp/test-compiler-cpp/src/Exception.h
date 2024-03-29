#pragma once

#include <exception>
#include "util/Util.h"

struct Exception : public std::exception {
public:
	Exception(const string& msg) : exception(msg.c_str()) {}
};

struct InvalidArgumentException : public Exception {
public:
	InvalidArgumentException(const string& msg) : Exception(msg) {}
};

struct NullptrException : public Exception {
public:
	NullptrException(const string& msg = "") : Exception(msg) {}
};

struct NotImplementedException : public Exception {
public:
	NotImplementedException(const string& file, const size_t line, const string& msg = "")
		: Exception("\"" + file + "\":" + std::to_string(line) + " Not implemented '" + msg + "'") {}
};

struct IncorrectImplException : public Exception {
public:
	IncorrectImplException(const string& file, const size_t line, const string& msg = "")
		: Exception("\"" + file + "\":" + std::to_string(line) + " found a bug: '" + msg + "'") {}
};
