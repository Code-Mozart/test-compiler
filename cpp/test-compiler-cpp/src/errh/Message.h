#pragma once

#include "util/Util.h"
#include "Common.h"

enum class Severity {
	Info, Warning, Error
};

const string ToString(Severity sev);

class Message {
public:
	const string text;
	const Severity severity;

	const string filepath;
	const ulong line;
	const ulong pos;
public:
	Message(const string& text, Severity severity, string filepath = "", ulong line = NoLine, ulong pos = NoPos);
	inline bool IsAboutFile() const { return filepath != NoFile; }
	inline bool IsAboutLine() const { return line != NoLine; }
	inline bool IsAboutPos() const { return pos != NoPos; }
};
