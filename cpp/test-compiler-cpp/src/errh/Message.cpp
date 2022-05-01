#include "Message.h"

#include "Exception.h"

const string ToString(Severity sev) {
	switch (sev) {
	case Severity::Info:	return "[i]";
	case Severity::Warning:	return "/!\\";
	case Severity::Error:	return "(x)";
	}
	throw IncorrectImplException(__FILE__, __LINE__, "enum case not handled");
}

Message::Message(const string& text, Severity severity, string filepath, ulong line, ulong pos)
	: text(text), severity(severity), filepath(filepath), line(line), pos(pos) {}
