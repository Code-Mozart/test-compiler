#include "ErrorHandler.h"

void ErrorHandler::Push(const string& text, Severity sev, const string& filepath, ulong line, ulong pos) {
	messages.emplace_back(text, sev, filepath, line, pos);
}

void ErrorHandler::PushErrImpl(const string& text, const string& filepath, ulong line, ulong pos) {
	compileFailed = true;
	Push(text, Severity::Error, filepath, line, pos);
}
