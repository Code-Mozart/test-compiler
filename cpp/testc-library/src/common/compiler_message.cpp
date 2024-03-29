#include "compiler_message.h"
#include "util/string_util.h"
#include "basic/memory.h"

namespace testc {

	static const string Info_Type_String		= "Info";
	static const string Warning_Type_String		= "Warning";
	static const string Error_Type_String		= "Error";

	static const string Info_String			= "[Info]";
	static const string Warning_String		= "[Warning]";
	static const string Error_String		= "[Error]";

    const string& as_string(Severity sev) {
	switch (sev) {
	case Severity::Info:		return Info_String;
	case Severity::Warning:		return Warning_String;
	case Severity::Error:		return Error_String;
	default:
		MISSING_ENUM(to_string(sev));
	}
	}

	const string& to_string(Severity sev) {
		switch (sev) {
		case Severity::Info:		return Info_Type_String;
		case Severity::Warning:		return Warning_Type_String;
		case Severity::Error:		return Error_Type_String;
		default:
			MISSING_ENUM(to_string((int)sev));
		}
	}

    string as_string(const Compiler_Message& msg) {
		return as_string(msg.severity) + append_non_empty(as_string(msg.location)) + append_non_empty(msg.text);
	}

    const Owner<const Compiler_Message> raise_warning(const string& text) {
		return new (allocate<Compiler_Message>()) Compiler_Message{ Severity::Warning, Location{}, text };
	}

    const Owner<const Compiler_Message> raise_error(const string& text) {
		return new (allocate<Compiler_Message>()) Compiler_Message{ Severity::Error, Location{}, text };
	}

    const Owner<const Compiler_Message> raise_error(const string& text, const Location& location) {
		return new (allocate<Compiler_Message>()) Compiler_Message{
			Severity::Error,
			Location{ location.filepath, location.line, location.pos },
			text
		}; 
	}
	
    const Owner<const Compiler_Message> raise_error(const string& text, Ref<const File> file, ulong line, ulong pos) {
		return new (allocate<Compiler_Message>()) Compiler_Message{
			Severity::Error,
			Location{ file, line, pos },
			text
		};
	}

}
