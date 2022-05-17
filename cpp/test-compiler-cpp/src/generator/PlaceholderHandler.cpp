#include "PlaceholderHandler.h"

#include "Exception.h"

short PlaceholderHandler::AddUsage(const string& identifier, size_t instructionIndex) {
    auto iter = resolvedIdentifiers.find(identifier);
    if (iter != resolvedIdentifiers.end()) {
        return iter->second;
    }
    else {
        pendingUsages[identifier].emplace_back(instructionIndex);
        return -1;
    }
}

void PlaceholderHandler::Resolve(const string& identifier, short line, vector<Instruction>& instructions) {
    if (line < 0) throw InvalidArgumentException("line must be positive");

    auto [resolvedIter, success] = resolvedIdentifiers.emplace(std::make_pair(identifier, line));
    if (!success) throw IncorrectImplException(__FILE__, __LINE__, "tried to resolve an identifier thats already resolved");

    auto usagesIter = pendingUsages.find(identifier);
    if (usagesIter != pendingUsages.end()) {
        for (auto& instrIdx : usagesIter->second) {
            // add 1 because the call is an instrution itself
            instructions[instrIdx].arg = line;
        }
        pendingUsages.erase(usagesIter);
    }
}
