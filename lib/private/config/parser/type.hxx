#pragma once

/**
 * @namespace Parser
 * @brief Parser namespace
 */
namespace StormByte::Config::Parser {
	/**
	 * @enum Mode
	 * @brief Parsing mode
	 */
	enum class Mode: unsigned short {
		Named, 		///< Named items
		Unnamed 	///< Unnamed items
	};

	/**
	 * @enum CommentType
	 * @brief Comment type
	 */
	enum class CommentType: unsigned short {
		None, 			///< No comment
		SingleLineBash, ///< SingleLine bash like comment
		SingleLineC, 	///< SingleLine C/C++ comment
		MultiLineC 		///< MultiLine C/C++ comment
	};
};