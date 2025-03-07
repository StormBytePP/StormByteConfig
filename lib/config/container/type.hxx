#pragma once

#include <config/exception.hxx>

namespace StormByte::Config::Container {
	/**
	 * @enum Type
	 * @brief Container type
	 */
	enum class STORMBYTE_CONFIG_PUBLIC Type: unsigned short {
		Group, 	///< Group of items
		List	///< List of items
	};

	/**
	 * Gets strings from Type
	 * @param t type to convert
	 * @return string
	 */
	constexpr STORMBYTE_CONFIG_PUBLIC const char*	TypeAsString(const Type& t) noexcept {
		switch(t) {
			case Type::Group:	return "Group";
			case Type::List:	return "List";
			default:			return "Unknown";
		}
	}

	/**
	 * Gets Type from start character
	 * @param start start character
	 * @return Type Container type
	 */
	constexpr STORMBYTE_CONFIG_PUBLIC Type			TypeFromStartCharacter(const char& start) noexcept {
		switch(start) {
			case '{': 			return Type::Group;
			case '[': 			return Type::List;
			default: 			throw Exception("Unknown start character " + std::string(1, start) + " for container");
		}
	}
}