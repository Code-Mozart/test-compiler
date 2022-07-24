#pragma once

#include <exception>
#include "basic/types.h"
#include "basic/macros.h"

namespace testc {

	struct Exception : public std::exception {
	public:
		Exception(const string& msg) : exception(msg.c_str()) {}
	protected:
		Exception(const string& name, const string& msg)
			: exception(("[" + name + "] " + msg).c_str()) {}
	};

	struct Invalid_Argument_Exception : public Exception {
	public:
		Invalid_Argument_Exception(const string& msg)
			: Exception(STRINGIFY(Invalid_Argument_Exception), msg) {}
	};

	struct Nullptr_Exception : public Exception {
	public:
		Nullptr_Exception(const string& msg = "")
			: Exception(STRINGIFY(Nullptr_Exception), msg) {}
	};

	struct Not_Supported_Exception : public Exception {
	public:
		Not_Supported_Exception()
			: Exception(STRINGIFY(Not_Supported_Exception), "") {}
		Not_Supported_Exception(const string& what)
			: Exception(STRINGIFY(Not_Supported_Exception),
				what + "is not supported") {}
	};

	struct Not_Implemented_Exception : public Exception {
	public:
		Not_Implemented_Exception(const string& file, const size_t line)
			: Exception("\"" + file + "\":" + std::to_string(line) + " Not implemented") {}
		Not_Implemented_Exception(const string& file, const size_t line, const string& msg)
			: Exception("\"" + file + "\":" + std::to_string(line) + " Not implemented '" + msg + "'") {}
	};
	#define NOT_IMPL() throw Not_Implemented_Exception(__FILE__, __LINE__)

	struct Incorrect_Impl_Exception : public Exception {
	public:
		Incorrect_Impl_Exception(const string& file, const size_t line, const string& msg = "")
			: Exception("\"" + file + "\":" + std::to_string(line) + " found a bug: '" + msg + "'") {}
	};
	#define INCORRECT_IMPL(msg) throw Incorrect_Impl_Exception(__FILE__, __LINE__, msg)
	#define MISSING_ENUM(missing) INCORRECT_IMPL("enum case " + missing + " not handled")
	
	struct Allocation_Exception : public Exception {
	public:
		Allocation_Exception(const string& file, const size_t line, const size_t size)
			: Exception("\"" + file + "\":" + std::to_string(line) + " Failed to allocate "
				+ std::to_string(size) + " bytes") {}
	};
	#define ALLOC_FAILED(size) throw Allocation_Exception(__FILE__, __LINE__, size)

}
