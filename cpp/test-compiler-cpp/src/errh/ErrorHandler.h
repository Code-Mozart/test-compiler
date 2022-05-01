#pragma once

#include "util/Util.h"
#include "Message.h"
#include "syntax/Token.h"
#include "semantic/ast/Node.h"

class ErrorHandler {
public:
	inline void PushInfo(const string& text, const string& filepath = NoFile, ulong line = NoLine, ulong pos = NoPos) {
		Push(text, Severity::Info, filepath, line, pos);
	}
	inline void PushInfo(const string& text, const Token& tkn) {
		Push(text, Severity::Info, tkn.filepath, tkn.line, tkn.pos);
	}
	inline void PushInfo(const string& text, const AST::Node& node) {
		Push(text, Severity::Info, node.filepath, node.line, node.pos);
	}

	inline void PushWarning(const string& text, const string& filepath = NoFile, ulong line = NoLine, ulong pos = NoPos) {
		Push(text, Severity::Warning, filepath, line, pos);
	}
	inline void PushWarning(const string& text, const Token& tkn) {
		Push(text, Severity::Warning, tkn.filepath, tkn.line, tkn.pos);
	}
	inline void PushWarning(const string& text, const AST::Node& node) {
		Push(text, Severity::Warning, node.filepath, node.line, node.pos);
	}

	inline void PushErr(const string& text, const string& filepath = NoFile, ulong line = NoLine, ulong pos = NoPos) {
		PushErrImpl(text, filepath, line, pos);
	}
	inline void PushErr(const string& text, const Token& tkn) {
		PushErrImpl(text, tkn.filepath, tkn.line, tkn.pos);
	}
	inline void PushErr(const string& text, const AST::Node& node) {
		PushErrImpl(text, node.filepath, node.line, node.pos);
	}

	inline vector<Message>::const_iterator begin() const { return messages.begin(); }
	inline vector<Message>::const_iterator end() const { return messages.end(); }
private:
	void Push(const string& text, Severity sev, const string& filepath, ulong line, ulong pos);
	void PushErrImpl(const string& text, const string& filepath, ulong line, ulong pos);
private:
	vector<Message> messages;
	bool compileFailed = false;
};
