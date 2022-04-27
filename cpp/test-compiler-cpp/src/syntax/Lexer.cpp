#include "Lexer.h"

static const vector<char> delimiters = {
	' ', '\n', '\t',
	';',
	'(', ')', '{', '}',
	'=', '+', '-', '*', '/', '<', '>'
};

inline static bool IsInt(char c) { return (c >= '0' && c <= '9'); }
inline static bool IsValidFirstChar(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
inline static bool IsValidChar(char c) { return IsValidFirstChar(c) || IsInt(c); }

vector<Token> Lexer::Tokenize(const FileInfo& srcFile) {
	vector<Token> tokens;
	const string& src = srcFile.content;
	if (srcFile.content.empty()) return tokens;

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
				errh.PushErr("unexpected EOF", srcFile.filename, line, pos);
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
					srcFile.filename,
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
					srcFile.filename,
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

			case '-':
			case '=':
			case '+':
			case '*':
			case '/':
			case '<':
			case '>':	type = TokenType::Operator;		break;

			case '\n':	line++; pos = 0;

			default:	continue;
			}
			tokens.emplace_back(
				srcFile.filename,
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
				// @improve: check if c is a valid char
				text += c;
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
					errh.PushErr("illegal char", srcFile.filename, line, pos);
					return {};
				}
			}
		}
	}

	return tokens;
}
