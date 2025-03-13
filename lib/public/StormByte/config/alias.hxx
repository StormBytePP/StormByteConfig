#pragma once

#include <functional>
#include <optional>
#include <vector>

namespace StormByte::Config {
	namespace Item {
		class Group;														// Forward declaration
	}
	using HookFunction 			= std::function<void(Item::Group&)>;		///< Hook function
	using HookFunctions 		= std::vector<HookFunction>;				///< Hook functions
	using OnFailureHook			= std::function<bool(const Item::Group&)>;	///< On failure function
	using OptionalFailureHook	= std::optional<OnFailureHook>;				///< Optional failure hook
}