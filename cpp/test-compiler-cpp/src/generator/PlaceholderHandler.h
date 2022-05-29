#pragma once

#include "util/Util.h"
#include "generator/Instruction.h"

// @later: when local procedures exist resolved identifiers must handle them correctly
class PlaceholderHandler {
public:
    // returns the line of the identifier or -1 when unresolved in which case
    // it is remembered as pending
	short AddUsage(const string& identifier, size_t instructionIndex);

    // resolves the given identifier and remembers its line for later usages,
    // resolves all pending usages
	void Resolve(const string& identifier, short line, vector<Instruction>& instructions);

    // call this method when entering a scope only if instruciton indices
    // added after here have to change when exiting the scope
    void EnterScope();

    // call this method when exiting a scope and instruction indices added
    // in this scope have to be offset now
    void ExitScope(size_t offset);

    // asserts that at the time of the call all usages are resolved
    // throws an IncorrectImplException if the assertion fails
    void AssertNothingUnresolved() const;
private:
    using PendingUsages = map<string, vector<size_t>>;

    map<string, short> resolvedIdentifiers;
	vector<PendingUsages> pendingUsagesStack;
};
