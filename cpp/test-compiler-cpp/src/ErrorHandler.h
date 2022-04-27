#pragma once

#include "util/Util.h"
#include "Message.h"

class Token;
namespace AST { class Node; }

class ErrorHandler {
public:
	void PushInfo(const string& text, const string& filepath);
	void PushInfo(const string& text, const Token& tkn);
	void PushInfo(const string& text, const AST::Node& node);

	void PushWarning(const string& text, const string& filepath);
	void PushWarning(const string& text, const Token& tkn);
	void PushWarning(const string& text, const AST::Node& node);

	void PushErr(const string& text);
	void PushErr(const string& text, const string& filepath);
	void PushErr(const string& text, const string& filepath, ulong line, ulong pos);
	void PushErr(const string& text, const Token& tkn);
	void PushErr(const string& text, const AST::Node& node);

	inline vector<Message>::const_iterator begin() const { return messages.begin(); }
	inline vector<Message>::const_iterator end() const { return messages.end(); }
private:
	void Push(const string& text, Severity sev, const string& filepath, ulong line, ulong pos);
private:
	vector<Message> messages;
	bool compileFailed = false;
};
