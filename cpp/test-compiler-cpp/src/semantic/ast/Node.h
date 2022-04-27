#pragma once

#include "util/Util.h"
#include "semantic/ast/ASTType.h"

namespace AST {

	class Node {
	public:
		virtual Type GetType() const = 0;
		inline virtual string ToString() const {
			return ToString("pos") + " " + AST::ToString(GetType());
		}
		inline virtual string ToString(const string& tag) const {
			if (tag == "pos") return filepath + ":" + std::to_string(line) + "," + std::to_string(pos);
			else return "";
		}
		inline virtual string TreeToString(byte indent = 0) const {
			return GetIndentString(indent) + Node::ToString();
		}
	protected:
		inline static string GetIndentString(byte indent) {
			string s;
			for (byte i = 0; i < indent; i++) s += "|  ";
			return s;
		}
	public:
		string filepath;
		ulong line;
		ulong pos;
	};

}
