#pragma once

#include "basic/include.h"

namespace testc {

	// This class can be used to reference a section of a character string
	// without the need for a copy.
	struct String_View {
		// A pointer to the start of the substring.
		Nullable<Array_Ref<const char>> pStart;

		// The length of the referenced substring.
		size_t len;

		// An empty string view which is a string view that is not referencing
		// any string.
		static const String_View Empty;


		bool operator==(const String_View&) const;
		// operator==(const string&) const;
		operator string() const;
	};

	string operator+(const String_View&, const char*);
	string operator+(const char*, const String_View&);
	string operator+(const string&, const String_View&);

}
