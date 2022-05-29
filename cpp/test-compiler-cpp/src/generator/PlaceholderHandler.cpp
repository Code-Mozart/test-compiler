#include "PlaceholderHandler.h"

#include "Exception.h"

short PlaceholderHandler::AddUsage(const string& identifier, size_t instructionIndex) {
    ASSERT(!pendingUsagesStack.empty());
    
    auto iter = resolvedIdentifiers.find(identifier);
    if (iter != resolvedIdentifiers.end()) {
        return iter->second;
    }
    else {
        pendingUsagesStack.back()[identifier].emplace_back(instructionIndex);
        return -1;
    }
}

void PlaceholderHandler::Resolve(const string& identifier, short line, vector<Instruction>& instructions) {
    if (line < 0) throw InvalidArgumentException("line must be positive");

    ASSERT(!pendingUsagesStack.empty());

    auto [resolvedIter, success] = resolvedIdentifiers.emplace(std::make_pair(identifier, line));
    if (!success) throw IncorrectImplException(__FILE__, __LINE__, "tried to resolve an identifier thats already resolved");

    PendingUsages& pendingUsages = pendingUsagesStack.back();
    auto usagesIter = pendingUsages.find(identifier);
    if (usagesIter != pendingUsages.end()) {
        for (auto& instrIdx : usagesIter->second) {
            // add 1 because the call is an instrution itself
            instructions[instrIdx].arg = line;
        }
        pendingUsages.erase(usagesIter);
    }
}

void PlaceholderHandler::EnterScope() {
    pendingUsagesStack.emplace_back();
}

void PlaceholderHandler::ExitScope(size_t offset) {
    if (pendingUsagesStack.empty()) {
        throw IncorrectImplException(__FILE__, __LINE__, "exiting scope while not inside a scope");
    }
    else if (pendingUsagesStack.size() == 1) {
        throw IncorrectImplException(__FILE__, __LINE__, "exiting the global scope");
    }

    PendingUsages& scopeUsages = pendingUsagesStack.back();
    PendingUsages& parentUsages = pendingUsagesStack[pendingUsagesStack.size() - 2];
    for (const auto& [identifier, usages] : scopeUsages) {
        vector<size_t>& instrIndices = parentUsages[identifier];
        for (size_t index : usages) {
            instrIndices.emplace_back(index + offset);
        }
    }
    pendingUsagesStack.pop_back();
}

void PlaceholderHandler::AssertNothingUnresolved() const {
    for (const auto& usages : pendingUsagesStack) {
        if (!usages.empty()) {
            throw IncorrectImplException(__FILE__, __LINE__, "expected all usages to be resolved now");
        }
    }
}
