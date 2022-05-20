#include "Lexer.h"

static const vector<char> delimiters = {
	' ', '\n', '\t',
	';',
	'(', ')', '{', '}',
	'=', '<', '>',
	'+', '-', '*', '/'
};

inline static bool IsInt(char c) { return (c >= '0' && c <= '9'); }
inline static bool IsValidFirstChar(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
inline static bool IsValidChar(char c) { return IsValidFirstChar(c) || IsInt(c); }

Lexer::Lexer(const FileInfo& srcInfo, ErrorHandler* errh)
	:
	srcInfo(srcInfo),
	errh(errh),
	tokens(Tokenize()),
	index(0)
{
}

bool Lexer::HasCurrent() const {
	return index < tokens.size();
}

bool Lexer::HasNext() const {
	return index + 1 < tokens.size();
}

const Token* Lexer::Peek() {
	if (index < tokens.size()) {
		return &(tokens[index]);
	}
	else {
		PushErr("unexpected EOF", srcInfo.filename, srcInfo.lines, tokens.back().pos + tokens.back().text.length());
		return nullptr;
	}
}

const Token* Lexer::PeekNext() {
	if (index + 1 < tokens.size()) {
		return &(tokens[index + 1]);
	}
	else {
		PushErr("unexpected EOF", srcInfo.filename, srcInfo.lines, tokens.back().pos + tokens.back().text.length());
		return nullptr;
	}
}

const Token* Lexer::Advance() {
	const Token* next = Peek();
	index++;
	return next;
}

const Token* Lexer::AdvanceAndPeek()
{
	index++;
	return Peek();
}

void Lexer::PushErr(const string& text, const string& filepath, ulong line, ulong pos) {
	if (errh) errh->PushErr(text, filepath, line, pos);
}

const vector<Token> Lexer::Tokenize() {
	vector<Token> tokens;
	const string& src = srcInfo.content;
	if (src.empty()) return tokens;

	ulong line = 1;
	ulong pos = 0;

	bool comment = false;
	bool skipLine = false;
	bool intLiteral = false;

	string text;
	short value = 0;

	for (int i = 0; i < src.size(); i++)
	{
		char c = src[i];
		pos++;
		if (c == '\r')
			continue;

		if (skipLine)
		{
			if (c == '\n')
			{
				skipLine = false;
				line++;
				pos = 0;
			}
			continue;
		}
		else if (comment)
		{
			if (src[i - 1] == '}' && c == '/')
				comment = false;
			continue;
		}
		else if (c == '/')
		{
			if (i >= src.size() - 1) {
				PushErr("unexpected EOF", srcInfo.filename, line, pos);
				return {};
			}
			else if (src[i + 1] == '/')
			{
				skipLine = true;
				i++;
				continue;
			}
			else if (src[i + 1] == '{')
			{
				comment = true;
				i++;
				continue;
			}
		}

		bool delimMatch = false;
		for (int j = 0; j < delimiters.size(); j++)
		{
			if (c == delimiters[j])
			{
				delimMatch = true;
				break;
			}
		}
		if (delimMatch)
		{
			if (intLiteral) {
				tokens.emplace_back(
					srcInfo.filename,
					line,
					pos - text.length(),

					TokenType::Literal,
					text,
					value
				);
				value = 0;
				intLiteral = false;
			}
			else if (!text.empty())
			{
				tokens.emplace_back(
					srcInfo.filename,
					line,
					pos - text.length(),

					TokenType::Identifier,
					text,
					SHRT_MIN
				);
			}
			text.clear();

			TokenType type;
			switch (c)
			{
			case ';':	type = TokenType::Semicolon;	break;

			case '(':	type = TokenType::LParen;		break;
			case ')':	type = TokenType::RParen;		break;
			case '{':	type = TokenType::LBrace;		break;
			case '}':	type = TokenType::RBrace;		break;

			case '+':
			case '-':
			case '*':
			case '/':

			case '=':
			case '<':
			case '>':	type = TokenType::Operator;		break;

			case '\n':	line++; pos = 0;

			default:	continue;
			}
			tokens.emplace_back(
				srcInfo.filename,
				line,
				pos,

				type,
				string() + src[i],
				SHRT_MIN
			);
		}
		else
		{
			if (intLiteral)
			{
				// @improve: check if c is between 0-9
				// @improve: prevent overflow
				value = value * 10 + (c - '0');
				text += c;
			}
			else if (!text.empty())
			{
				if (IsValidChar(c))
				{
					text += c;
				}
				else
				{
					PushErr(string() + "illegal char '" + c + "'", srcInfo.filename, line, pos);
					return {};
				}
			}
			else
			{
				if (IsInt(c))
				{
					value = (c - '0');
					text += c;
					intLiteral = true;
				}
				else if (IsValidFirstChar(c))
				{
					text += c;
				}
				else
				{
					PushErr(string() + "illegal char '" + c + "'", srcInfo.filename, line, pos);
					return {};
				}
			}
		}
	}

	return tokens;
}
