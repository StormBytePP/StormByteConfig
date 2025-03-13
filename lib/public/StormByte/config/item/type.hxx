#pragma once

#include <StormByte/config/exception.hxx>
#include <StormByte/config/visibility.h>

#include <concepts>
#include <string>
#include <type_traits>

/**
 * @namespace Item
 * @brief All the configuration item classes namespace
 */
namespace StormByte::Config::Item {
	/**
	 * @enum Type
	 * @brief Item type
	 */
	enum class Type: unsigned short {
		Bool,       ///< Bool item
		Comment,    ///< Comment item
		Container,  ///< Container item
		Double,     ///< Double item
		Integer,    ///< Integer item
		String      ///< String item
	};


	/**
	 * Gets strings from Type
	 * @param t type to convert
	 * @return string
	 */
	static constexpr const char* TypeToString(const Type& t) noexcept {
		switch(t) {
			case Type::String: 		return "String";
			case Type::Integer:		return "Integer";
			case Type::Double: 		return "Double";
			case Type::Comment:		return "Comment";
			case Type::Bool:		return "Bool";
			case Type::Container:	return "Container";
			default:				return "Unknown";
		}
	}

	/**
	 * @enum Type
	 * @brief Comment type
	 */
	enum class STORMBYTE_CONFIG_PUBLIC CommentType: unsigned short {
		SingleLineBash, 	///< Single line comment starting with # (bash style)
		SingleLineC,		///< Single line comment starting with // (C++ style)
		MultiLineC			///< Multi line comment starting with /* and ending with */ (C/C++ style)
	};

	/**
	 * Gets strings from Type
	 * @param t type to convert
	 * @return string
	 */
	constexpr std::string STORMBYTE_CONFIG_PUBLIC TypeToString(const CommentType& t) noexcept {
		switch(t) {
			case CommentType::SingleLineBash: 	return "Bash like single line comment";
			case CommentType::SingleLineC: 		return "C++ like single line comment";
			case CommentType::MultiLineC: 		return "C/C++ like multi line comment";
			default: 							return "Unknown";
		}
	}

	/**
	 * @enum Type
	 * @brief Container type
	 */
	enum class STORMBYTE_CONFIG_PUBLIC ContainerType: unsigned short {
		Group, 	///< Group of items
		List	///< List of items
	};

	/**
	 * Gets strings from Type
	 * @param t type to convert
	 * @return string
	 */
	constexpr STORMBYTE_CONFIG_PUBLIC const char* TypeToString(const ContainerType& t) noexcept {
		switch(t) {
			case ContainerType::Group:	return "Group";
			case ContainerType::List:	return "List";
			default:					return "Unknown";
		}
	}

	/**
	 * Gets Type from start character
	 * @param start start character
	 * @return Type Container type
	 */
	constexpr STORMBYTE_CONFIG_PUBLIC ContainerType TypeFromStartCharacter(const char& start) noexcept {
		switch(start) {
			case '{': 			return ContainerType::Group;
			case '[': 			return ContainerType::List;
			default: 			throw Exception("Unknown start character " + std::string(1, start) + " for container");
		}
	}

	template<typename T>
    concept AllowedValueType = std::is_same_v<T, int> || 
                          std::is_same_v<T, double> || 
                          std::is_same_v<T, bool> || 
                          std::is_same_v<T, std::string>;
}