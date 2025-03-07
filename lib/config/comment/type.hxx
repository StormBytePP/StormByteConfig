#pragma once

#include <config/visibility.h>

#include <string>

/**
 * @namespace Comment
 * @brief All the classes for handling comments on configuration files
 */
namespace StormByte::Config::Comment {
	/**
	 * @enum Type
	 * @brief Comment type
	 */
	enum class STORMBYTE_CONFIG_PUBLIC Type: unsigned short {
		SingleLine, ///< Single line comment starting with #
		MultiLine	///< Multi line comment starting with /* and ending with */
	};

	/**
	 * Gets strings from Type
	 * @param t type to convert
	 * @return string
	 */
	constexpr std::string STORMBYTE_CONFIG_PUBLIC TypeAsString(const Type& t) noexcept {
		switch(t) {
			case Type::SingleLine: 	return "SingleLine";
			case Type::MultiLine: 	return "MultiLine";
			default: 				return "Unknown";
		}
	}
}