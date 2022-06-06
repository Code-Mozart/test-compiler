#pragma once

#include <exception>
#include "util/Util.h"

struct Exception : public std::exception {
public:
	Exception(const string& msg) : exception(msg.c_str()) {}
protected:
	Exception(const string& name, const string& msg)
		: exception(("[" + name + "] " + msg).c_str()) {}
};

struct InvalidArgumentException : public Exception {
public:
	InvalidArgumentException(const string& msg)
		: Exception(STRINGIFY(InvalidArgumentException), msg) {}
};

struct NullptrException : public Exception {
public:
	NullptrException(const string& msg = "")
		: Exception(STRINGIFY(NullptrException), msg) {}
};

struct NotSupportedException : public Exception {
public:
	NotSupportedException()
		: Exception(STRINGIFY(NotSupportedException), "") {}
	NotSupportedException(const string& what)
		: Exception(STRINGIFY(NotSupportedException),
			what + "is not supported") {}
};

struct NotImplementedException : public Exception {
public:
	NotImplementedException(const string& file, const size_t line, const string& msg = "")
		: Exception("\"" + file + "\":" + std::to_string(line) + " Not implemented '" + msg + "'") {}
};
#define NOT_IMPL() throw NotImplementedException(__FILE__, __LINE__)

struct IncorrectImplException : public Exception {
public:
	IncorrectImplException(const string& file, const size_t line, const string& msg = "")
		: Exception("\"" + file + "\":" + std::to_string(line) + " found a bug: '" + msg + "'") {
		NOT_IMPL();
	}
};
#define INCORRECT_IMPL(msg) throw IncorrectImplException(__FILE__, __LINE__, msg)
#define MISSING_ENUM(missing) INCORRECT_IMPL("enum case " + missing + " not handled");
