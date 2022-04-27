#include "RuntimeStack.h"

void RuntimeStack::PutVar(const string& identifier)
{
	auto [iter, success] = locations.emplace(std::make_pair(identifier, std::make_pair(scopes.back(), size)));
	if (!success) throw IncorrectImplException(__FILE__, __LINE__,
		"multiple occurences of the same var on the predicted runtime stack");
}

byte RuntimeStack::ExitScope()
{
	if (scopes.empty())
		throw IncorrectImplException(__FILE__, __LINE__, "exiting scope, while not inside any scope");

	byte count = 0;
	auto iter = locations.begin();
	while (iter != locations.end()) {
		if (iter->second.first == scopes.back()) {
			iter = locations.erase(iter);
			count++;
			Shrink();
		}
		else {
			++iter;
		}
	}
	scopes.pop_back();
	return count;
}
