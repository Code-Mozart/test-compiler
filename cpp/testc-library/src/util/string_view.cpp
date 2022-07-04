#include "string_view.h"

namespace testc {

	const String_View String_View::Empty = String_View{ nullptr, 0 };
	
	bool String_View::operator==(const String_View& other) const {
		if (this == &other) {
			return true;
		}
		else if (this->pStart == nullptr) {
			return other.pStart == nullptr;
		}
		else if (this->len != other.len) {
			return false;
		}
		else {
			return strncmp(this->pStart, other.pStart, len);
		}
	}

	String_View::operator string() const {
		return pStart ? string(pStart, len) : string();
	}

	string operator+(const String_View& view, const char* cstr) {
		return view.operator string() + cstr;
	}

	string operator+(const char* cstr, const String_View& view) {
		return cstr + view.operator string();
	}

}
