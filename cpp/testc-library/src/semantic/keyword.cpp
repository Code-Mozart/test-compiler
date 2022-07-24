#include "keyword.h"

namespace testc {

	static const string Var_String = "var";
	static const string Proc_String = "proc";
	static const string If_String = "if";
	static const string Else_String = "else";
	static const string While_String = "while";

	static const List<Keyword> ordered = {
		Keyword::Else,
		Keyword::If,
		Keyword::Procedure_Decl,
		Keyword::Variable_Decl,
		Keyword::While,
	};
	
	const List<Keyword>& get_keywords_ordered() {
		return ordered;
	}

    const string& as_string(Keyword keyword) {
		switch (keyword) {
		case Keyword::Variable_Decl:		return Var_String;
		case Keyword::Procedure_Decl:		return Proc_String;
		case Keyword::If:					return If_String;
		case Keyword::Else:					return Else_String;
		case Keyword::While:				return While_String;
		default:
			MISSING_ENUM(to_string((int)keyword));
		}
    }

}
