#include "compiler_message.h"
#include "util/string_util.h"

namespace testc {

    const string& as_string(Severity sev) {
	switch (sev) {
	case Severity::Info:		return "[Info]";
	case Severity::Warning:		return "[Warning]";
	case Severity::Error:		return "[Error]";
	default:
		MISSING_ENUM(to_string(sev));
	}
	}

    string as_string(const Compiler_Message& msg) {
		return as_string(msg.severity) + append_non_empty(as_string(msg.location)) + append_non_empty(msg.text);
	}

    const Owner<const Compiler_Message> raise_error(const string& text) {
		return new Compiler_Message{ Severity::Error, Location{}, text };
	}

}
