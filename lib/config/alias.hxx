#pragma once

#include <functional>
#include <vector>

namespace StormByte::Config {
	namespace Item {
		class Group;											// Forward declaration
	}
	using HookFunction 	= std::function<void(Item::Group&)>;	///< Hook function
	using HookFunctions = std::vector<HookFunction>;			///< Hook functions
}