#pragma once

/**
 * @namespace Parser
 * @brief Parsing types
 */
namespace StormByte::Config::Parser {
	/**
	 * @enum Mode
	 * @brief Parsing mode
	 */
	enum class Mode: unsigned short {
		Named, 	///< Named items
		Unnamed ///< Unnamed items
	};

	/**
	 * @enum CommentType
	 * @brief Comment type
	 */
	enum class CommentType: unsigned short {
		None, 		///< No comment
		SingleLine, ///< SingleLine line comment
		MultiLine 	///< MultiLine line comment
	};
};