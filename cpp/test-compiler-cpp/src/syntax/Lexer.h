#pragma once

#include "Token.h"
#include "Exception.h"
#include "errh/ErrorHandler.h"
#include "io/FileInfo.h"

class Lexer {
public:
	Lexer(ErrorHandler& errh) : errh(errh) {}
public:
	vector<Token> Tokenize(const FileInfo& srcFile);
private:
	void PushErr(const string& text, const string& filepath, ulong line, ulong pos);
private:
	ErrorHandler& errh;
};

struct EOFException : public Exception {
public:
	EOFException(const string& filepath, const ulong line, const ulong pos, const string& msg = "")
		: Exception("unexpected EOF at " + filepath + ":" + std::to_string(line) + "," +
			std::to_string(pos)+ " '" + msg + "'") {}
};

struct IllegalCharException : public Exception {
public:
	IllegalCharException(const string& filepath, const ulong line, const ulong pos, const char c, const string& msg = "")
		: Exception(string("illegal char '") + c + "' at " + filepath + ":" + std::to_string(line) + "," +
			std::to_string(pos)+ " '" + msg + "'") {}
};
