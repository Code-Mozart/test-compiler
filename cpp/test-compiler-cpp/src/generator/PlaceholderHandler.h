#pragma once

#include "util/Util.h"
#include "generator/Instruction.h"

class PlaceholderHandler {
public:
    // returns the line of the identifier or -1 when unresolved in which case
    // it is remembered as pending
	short AddUsage(const string& identifier, size_t instructionIndex);

    // resolves the given identifier and remembers its line for later usages,
    // resolves all pending usages
	void Resolve(const string& identifier, short line, vector<Instruction>& instructions);
private:
    map<string, short> resolvedIdentifiers;
	map<string, vector<size_t>> pendingUsages;
};
