#include "operator.h"

#include "exception.h"

namespace testc {

	static const string Plus_String			= "+";
	static const string Minus_String		= "-";
	static const string Asterisk_String		= "*";
	static const string Slash_String		= "/";
	static const string Less_Than_String	= "<";

	const string& as_string(Operator op) {
		switch (op) {
		case Operator::Plus:			return Plus_String;
		case Operator::Minus:			return Minus_String;
		case Operator::Asterisk:		return Asterisk_String;
		case Operator::Slash:			return Slash_String;
		case Operator::Less_Than:		return Less_Than_String;
		default:
			MISSING_ENUM(to_string((int)op));
		}
	}

}
