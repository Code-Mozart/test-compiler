#pragma once

#include "basic/include.h"

namespace testc {

	inline string append_non_empty(const string& s) {
		return (s.empty()) ? "" : " " + s;
	}

	inline string operator+(const string& str, int n) { return str + to_string(n); }
	inline string operator+(const string& str, uint n) { return str + to_string(n); }
	inline string operator+(const string& str, ulong n) { return str + to_string(n); }

	inline string operator+(int n, const string& str) { return to_string(n) + str; }
	inline string operator+(uint n, const string& str) { return to_string(n) + str; }
	inline string operator+(ulong n, const string& str) { return to_string(n) + str; }

}
