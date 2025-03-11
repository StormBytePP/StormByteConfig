#pragma once

#include <alias.hxx>
#include <config/alias.hxx>
#include <config/exception.hxx>
#include <config/item/comment.hxx>
#include <config/item/group.hxx>
#include <config/item/list.hxx>
#include <config/item/value.hxx>
#include <config/parser/type.hxx>
#include <config/type.hxx>

#include <istream>
#include <regex>
#include <string>

/**
 * @namespace Parser
 * @brief Parser namespace
 */
namespace StormByte::Config::Parser {
	class STORMBYTE_CONFIG_PRIVATE Parser {
		public:
			/**
			 * Copy constructor
			 * @param p parser to copy
			 */
			Parser(const Parser& p) 								= delete;

			/**
			 * Move constructor
			 * @param p parser to move
			 */
			Parser(Parser&& p) 										= default;

			/**
			 * Assignment operator
			 * @param p parser to copy
			 */
			Parser& operator=(const Parser& p) 						= delete;

			/**
			 * Move assignment operator
			 * @param p parser to move
			 */
			Parser& operator=(Parser&& p) 							= default;

			/**
			 * Destructor
			 */
			~Parser() 												= default;

			/**
			 * Parse a configuration file
			 * @param stream input stream
			 * @param root root group to start
			 * @param action action to take when a name is already in use
			 * @param before hooks to call before parsing
			 * @param after hooks to call after parsing
			 */
			static Expected<void, ParseError>						Parse(std::istream& stream, Item::Group& root, const OnExistingAction& action, const HookFunctions& before, const HookFunctions& after);

			/**
			 * Parse a configuration file
			 * @param string input string
			 * @param root root group to start
			 * @param action action to take when a name is already in use
			 * @return Group with parsed information
			 */
			static Expected<void, ParseError>						Parse(const std::string& string, Item::Group& root, const OnExistingAction& action, const HookFunctions& before, const HookFunctions& after);

		private:
			unsigned int 											m_container_level;					///< Container level
			unsigned int 											m_current_line;						///< Current line (for parser)
			const OnExistingAction 									c_on_existing_action;				///< Action to take when item name already exists
			static const std::regex 								c_double_regex;						///< Double detection regex
			static const std::regex 								c_int_regex;						///< Integer detection regex

			/**
			 * Constructor
			 * @param action action to take when a name is already in use
			 */
			Parser(const OnExistingAction& action);

			/**
			 * Starts parsing
			 * @param istream input stream
			 */
			void 													StartParse(std::istream& istream);

			/**
			 * Parses a value
			 * @param istream input stream
			 * @return parsed value
			 */
			template<typename T> Expected<T, ParseError>			ParseValue(std::istream& istream);

			/**
			 * Finds a comment and returns its type
			 * @param istream input stream
			 * @return comment type
			 */
			CommentType												FindComment(std::istream& istream);

			/**
			 * Finds and parses comments
			 * @param istream input stream
			 * @param container container to put comments to
			 */
			Expected<void, ParseError>								FindAndParseComments(std::istream& istream, Item::Container& container);

			/**
			 * Parses an item
			 * @param istream input stream
			 * @param type item type
			 * @return parsed item
			 */
			Expected<Item::Base::PointerType, ParseError>			ParseItem(std::istream& istream, const Item::Type& type);

			/**
			 * Parses a group
			 * @param istream input stream
			 * @param container Container to put data to
			 */
			Expected<void, ParseError>								Parse(std::istream& istream, Item::Container& container, const Mode& mode);

			/**
			 * Parses an item name
			 * @param istream input stream
			 * @return item name
			 */
			Expected<std::string, ParseError>						ParseItemName(std::istream& istream);

			/**
			 * Parses an item type
			 * @param istream input stream
			 * @return item type
			 */
			Expected<Item::Type, ParseError>						ParseType(std::istream& istream);

			/**
			 * Parses a container type
			 * @param istream input stream
			 * @return container type
			 */
			Expected<Item::ContainerType, ParseError>				ParseContainerType(std::istream& istream);

			/**
			 * Finds the container end symbol
			 * @param istream input stream
			 * @param container_type container type to know the end symbol
			 * @return bool
			 */
			bool 													FindContainerEnd(std::istream& istream, const Item::ContainerType& container_type);

			/**
			 * Consumes whitespace (and tabs, newline, etc) updating line number
			 * @param istream stream to consume whitespace from
			 */
			void 													ConsumeWS(std::istream& istream);

			/**
			 * Gets a string ignoring leading whitespace updating line number
			 * @param istream input stream
			 * @return string value
			 */
			std::string 											GetStringIgnoringWS(std::istream& istream);
	};
	/**
	 * Shortcut for Parser static Parse method
	 * @param stream input stream
	 * @param root root group to start
	 * @param action action to take when a name is already in use
	 * @throws ParserError If parse errors are found
	 * @return Group with parsed information
	 */
	Expected<void, ParseError> STORMBYTE_CONFIG_PRIVATE 			Parse(std::istream& stream, Item::Group& root, const OnExistingAction& action, const HookFunctions& before, const HookFunctions& after);

	/**
	 * Shortcut for Parser static Parse method
	 * @param string input string
	 * @param root root group to start
	 * @param action action to take when a name is already in use
	 * @throws ParserError If parse errors are found
	 * @return Group with parsed information
	 */
	Expected<void, ParseError> STORMBYTE_CONFIG_PRIVATE 			Parse(const std::string& string, Item::Group& root, const OnExistingAction& action, const HookFunctions& before, const HookFunctions& after);
}