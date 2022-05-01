#pragma once

#include "util/Util.h"
#include "Common.h"
#include "semantic/ast/ASTType.h"

namespace AST {

	class Node {
	public:
		virtual Type GetType() const = 0;
		virtual bool IsType(Type type) const {
			return ((short)type) & ((short)GetType());
		}
		inline virtual string ToString(byte indent = 0) const {
			return GetIndentString(indent) + filepath + ":"
				+ std::to_string(line) + "," + std::to_string(pos) + " "
				+ AST::ToString(GetType()) + "\n";
		}
	protected:
		inline static string GetIndentString(byte indent) {
			string s;
			for (byte i = 0; i < indent; i++) s += "|  ";
			return s;
		}
	public:
		string filepath = NoFile;
		ulong line = NoLine;
		ulong pos = NoPos;
	};

}
