#pragma once

#include "util/Util.h"

enum class Severity {
	Info, Warning, Error
};

struct Message {
	string text;
	Severity severity;

	string filepath;
	ulong line;
	ulong pos;

	Message(const string& text, Severity severity, string filepath, ulong line, ulong pos)
		: text(text), severity(severity), filepath(filepath), line(line), pos(pos) {}
};
