#include "ErrorHandler.h"

#include "syntax/Token.h"
#include "semantic/ast/Node.h"

// INFO

void ErrorHandler::PushInfo(const string& text, const string& filepath) {
	Push(text, Severity::Info, filepath, 0, 0);
}

void ErrorHandler::PushInfo(const string& text, const Token& tkn) {
	Push(text, Severity::Info, tkn.filepath, tkn.line, tkn.pos);
}

void ErrorHandler::PushInfo(const string& text, const AST::Node& node) {
	Push(text, Severity::Info, node.filepath, node.line, node.pos);
}

// WARNING

void ErrorHandler::PushWarning(const string& text, const string& filepath) {
	Push(text, Severity::Warning, filepath, 0, 0);
}

void ErrorHandler::PushWarning(const string& text, const Token& tkn) {
	Push(text, Severity::Warning, tkn.filepath, tkn.line, tkn.pos);
}

void ErrorHandler::PushWarning(const string& text, const AST::Node& node) {
	Push(text, Severity::Warning, node.filepath, node.line, node.pos);
}

// ERROR

void ErrorHandler::PushErr(const string& text) {
	Push(text, Severity::Error, "", 0, 0);
}

void ErrorHandler::PushErr(const string& text, const string& filepath) {
	Push(text, Severity::Error, filepath, 0, 0);
}

void ErrorHandler::PushErr(const string& text, const string& filepath, ulong line, ulong pos) {
	Push(text, Severity::Error, filepath, line, pos);
}

void ErrorHandler::PushErr(const string& text, const Token& tkn) {
	Push(text, Severity::Error, tkn.filepath, tkn.line, tkn.pos);
}

void ErrorHandler::PushErr(const string& text, const AST::Node& node) {
	Push(text, Severity::Error, node.filepath, node.line, node.pos);
}

// GENERAL

void ErrorHandler::Push(const string& text, Severity sev, const string& filepath, ulong line, ulong pos) {
	if (sev == Severity::Error) {
		compileFailed = true;
	}
	messages.emplace_back(text, sev, filepath, line, pos);
}
