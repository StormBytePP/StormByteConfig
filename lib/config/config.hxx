#pragma once

#include <config/container/group.hxx>
#include <config/parser.hxx>

#include <functional>

/**
 * @namespace Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	// Forward declaration
	namespace Comment {
		class MultiLine;
		class SingleLine;
	};
}

/**
 * @namespace Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	/**
	 * @class Config
	 * @brief Abstract class for a configuration file
	 * 
	 * The configuration specs are defined by its elements, have no "root" element and can contain any number of the below:
	 * - Bool
	 * - Double
	 * - Integer
	 * - String
	 * - SingleLine line comment (starting with #)
	 * - MultiLineple line comment (like C/C++ multiline comments)
	 * - Group
	 * - List
	 */
	class STORMBYTE_CONFIG_PUBLIC Config: private Container::Group {
		public:
			/**
			 * @class Iterator
			 * @brief Iterator for Config
			 */
			using Iterator = Group::Iterator;	///< Iterator for Config

			/**
			 * @class ConstIterator
			 * @brief Const iterator for Config
			 */
			using ConstIterator = Group::ConstIterator;	///< ConstIterator for Config

			/**
			 * Constructor
			 */
			constexpr Config():m_on_existing_action(OnExistingAction::ThrowException) {}

			/**
			 * Copy constructor
			 */
			constexpr Config(const Config&)					= default;

			/**
			 * Move constructor
			 */
			constexpr Config(Config&&) noexcept				= default;

			/**
			 * Assignment operator
			 */
			Config& operator=(const Config&)				= default;

			/**
			 * Move assignment operator
			 */
			constexpr Config& operator=(Config&&) noexcept	= default;

			/**
			 * Destructor
			 */
			virtual constexpr ~Config()	override			= default;

			/**
			 * Gets a reference to item by path
			 * @param path path to item
			 * @return item reference
			 */
			constexpr Item&									operator[](const std::string& path) override {
				return const_cast<Item&>(static_cast<const Config&>(*this)[path]);
			}

			/**
			 * Gets a const reference to item by path
			 * @param path path to item
			 * @return item const reference
			 */
			constexpr const Item&							operator[](const std::string& path) const override {
				return Group::operator[](path);
			}

			/**
			 * Gets a reference to item by index
			 * @param index index of item
			 * @throw OutOfBounds if index is out of bounds
			 * @return item reference
			 */
			constexpr Item& 								operator[](const size_t& index) override {
				return const_cast<Item&>(static_cast<const Config&>(*this)[index]);
			}

			/**
			 * Gets a const reference to item by index
			 * @param index index of item
			 * @throw OutOfBounds if index is out of bounds
			 * @return item const reference
			 */
			constexpr const Item& 							operator[](const size_t& index) const override {
				return Group::operator[](index);
			}

			/**
			 * Compares two configurations
			 * @param other configuration to compare to
			 * @return bool are they equal?
			 */
			constexpr bool 									operator==(const Config& other) const noexcept {
				return Group::operator==(other);
			}

			/**
			 * Compares two configurations
			 * @param other configuration to compare to
			 * @return bool are they not equal?
			 */
			constexpr bool 									operator!=(const Config& other) const noexcept {
				return !(*this == other);
			}

			/* INPUT */
			/**
			 * Take data from another configuration
			 * @param source source configuration to import
			 * @return Reference to configuration
			 */
			Config& 										operator<<(const Config& source);

			/**
			 * Initialize configuration with an input stream
			 * @param istream input stream
			 */
			void 											operator<<(std::istream& istream); // 1

			/**
			 * Initialize configuration with a string
			 * @param str input string
			 */
			void 											operator<<(const std::string& str); // 2

			/**
			 * Initializes configuration with istream (when istream is in the left part)
			 * @param istream input stream
			 * @param file Config to put data to
			 */
			friend STORMBYTE_CONFIG_PUBLIC Config&			operator>>(std::istream& istream, Config& file); // 3

			/**
			 * Initializes configuration with string (when string is in the left part)
			 * @param str input string
			 * @param file Config to put data to
			 */
			friend STORMBYTE_CONFIG_PUBLIC Config&			operator>>(const std::string& str, Config& file); // 4
			
			/* OUTPUT */
			/**
			 * Outut current configuration to another configuration
			 * @param dest configuration destination
			 * @return a reference to destination conf
			 */
			Config& 										operator>>(Config& dest) const;

			/**
			 * Output configuration serialized to output stream
			 * @param ostream output stream
			 */
			std::ostream&									operator>>(std::ostream& ostream) const; // 5

			/**
			 * Output configuration serialized to string
			 * @param str output string
			 */
			std::string&									operator>>(std::string& str) const; // 6

			/**
			 * Output configuration serialized to output stream (when output stream is in the left part)
			 * @param ostream output stream
			 * @param file Config to get data from
			 */
			friend STORMBYTE_CONFIG_PUBLIC std::ostream&	operator<<(std::ostream& ostream, const Config& file); // 7

			/**
			 * Output configuration serialized to string (when string is in the left part)
			 * @param str output string
			 * @param file Config to get data from
			 */
			friend STORMBYTE_CONFIG_PUBLIC std::string& 	operator<<(std::string&, const Config&); // 8

			/**
			 * Converts current configuration to string
			 */
															operator std::string() const;

			/**
			 * Adds an item to the container
			 * @param item item to add
			 * @throw ItemNameAlreadyExists if item name already exists
			 * @return reference to added item
			 */
			constexpr Item&									Add(const Item& item) {
				return Group::Add(Item(item), m_on_existing_action);
			}

			/**
			 * Adds an item to the container
			 * @param item item to add
			 * @throw ItemNameAlreadyExists if item name already exists
			 * @return reference to added item
			 */
			constexpr Item&									Add(Item&& item) {
				return Group::Add(std::move(item), m_on_existing_action);
			}

			/**
			 * Clears all configuration items
			 */
			inline void										Clear() noexcept {
				Group::Clear();
			}

			/**
			 * Checks if item exists by path
			 * @param path path to item
			 * @return bool exists?
			 */
			inline bool 									Exists(const std::string& path) const {
				return Group::Exists(path);
			}

			/**
			 * Removes an item by path
			 * @param path item path
			 * @throw ItemNotFound if item is not found
			 */
			inline void										Remove(const std::string& path) {
				Group::Remove(path);
			}

			/**
			 * Removes an item by position
			 * @param path item path
			 * @throw OutOfBounds if index is out of bounds
			 */
			constexpr void									Remove(const size_t& path) {
				Group::Remove(path);
			}

			/**
			 * Sets a function to execute on name collission when inserting
			 * @param on_existing function to select element on collission
			 * @see Container::OnExistingAction
			 */
			constexpr void									SetOnExistingAction(const OnExistingAction& on_existing) {
				m_on_existing_action = on_existing;
			}

			/**
			 * Adds a hook which will take Config as parameter and will be executed before read start
			 * Hooks will be executed *in order*
			 */
			inline void 									AddHookBeforeRead(std::function<void(Config&)> hook) {
				m_before_read_hooks.push_back(hook);
			}
			
			/**
			 * Adds a hook which will take Config as parameter and will be executed on successful read
			 * Hooks will be executed *in order*
			 */
			inline void 									AddHookAfterRead(std::function<void(Config&)> hook) {
				m_after_read_hooks.push_back(hook);
			}

			/**
			 * Gets an iterator pointing to the first element
			 * @return iterator
			 */
			constexpr Iterator								Begin() noexcept override {
				return Group::Begin();
			}

			/**
			 * Gets a const iterator pointing to the first element
			 * @return const iterator
			 */
			constexpr ConstIterator							Begin() const noexcept override {
				return Group::Begin();
			}

			/**
			 * Gets an iterator pointing to past last element
			 * @return iterator
			 */
			constexpr Iterator								End() noexcept override {
				return Group::End();
			}

			/**
			 * Gets a const iterator pointing to past last element
			 * @return const iterator
			 */
			constexpr ConstIterator							End() const noexcept override {
				return Group::End();
			}

			/**
			 * Gets the number of items in the current level
			 * @return size_t number of items
			 */
			virtual constexpr size_t						Size() const noexcept override {
				return Group::Size();
			}

			/**
			 * Gets the full number of items
			 * @return size_t number of items
			 */
			inline virtual size_t							Count() const noexcept override {
				return Group::Count();
			}

		protected:
			/**
			 * Ordered hook list which will be executed sequentially
			 * in their corresponding event
			 */
			std::vector<std::function<void(Config&)>> m_before_read_hooks, m_after_read_hooks;	///< Hooks executed before and after successful reading

			/**
			 * Function to override the default action when duplicate name is found when inserting
			 * Takes 3 parameters: current configuration, existing item and new item and will return
			 * the item to be inserted (or might throw to cancel the insert)
			 */
			OnExistingAction m_on_existing_action;												///< Action to take when item name already exists

		private:
			/**
			 * Container level
			 */
			unsigned int 									m_container_level;

			/**
			 * Current line
			 */
			unsigned int 									m_current_line;

			/**
			 * Starts parsing
			 * @param istream input stream
			 */
			void 											StartParse(std::istream& istream);

			/**
			 * Parses a value
			 * @param istream input stream
			 * @return parsed value
			 */
			template<typename T> T							ParseValue(std::istream& istream);
			#ifdef MSVC
			/**
			 * Parses a value
			 * @param istream input stream
			 * @return parsed value
			 */
			template<> Comment::MultiLine					ParseValue<Comment::MultiLine>(std::istream& istream);

			/**
			 * Parses a value
			 * @param istream input stream
			 * @return parsed value
			 */
			template<> Comment::SingleLine					ParseValue<Comment::SingleLine>(std::istream& istream);

			/**
			 * Parses a value
			 * @param istream input stream
			 * @return parsed value
			 */
			template<> double								ParseValue<double>(std::istream& istream);

			/**
			 * Parses a value
			 * @param istream input stream
			 * @return parsed value
			 */
			template<> int 									ParseValue<int>(std::istream& istream);

			/**
			 * Parses a value
			 * @param istream input stream
			 * @return parsed value
			 */
			template<> std::string 							ParseValue<std::string>(std::istream& istream);

			/**
			 * Parses a value
			 * @param istream input stream
			 * @return parsed value
			 */
			template<> bool 								ParseValue<bool>(std::istream& istream);
			#endif

			/**
			 * Finds a comment and returns its type
			 * @param istream input stream
			 * @return comment type
			 */
			Parser::CommentType								FindComment(std::istream& istream);

			/**
			 * Finds and parses comments
			 * @param istream input stream
			 * @param container container to put comments to
			 */
			void											FindAndParseComments(std::istream& istream, Container::Base& container);

			/**
			 * Parses an item
			 * @param istream input stream
			 * @param type item type
			 * @throw ParseError if item is invalid
			 * @return parsed item
			 */
			std::unique_ptr<Item>							ParseItem(std::istream& istream, const Item::Type& type);

			/**
			 * Parses a group
			 * @param istream input stream
			 * @param container Container to put data to
			 */
			void 											Parse(std::istream& istream, Container::Base& group, const Parser::Mode& mode);

			/**
			 * Parses an item name
			 * @param istream input stream
			 * @throw ParseError if name is invalid
			 * @return item name
			 */
			std::string 									ParseItemName(std::istream& istream);

			/**
			 * Parses an item type
			 * @param istream input stream
			 * @return item type
			 */
			Item::Type										ParseType(std::istream& istream);

			/**
			 * Parses a container type
			 * @param istream input stream
			 * @return container type
			 */
			Container::Type 								ParseContainerType(std::istream& istream);

			/**
			 * Finds the container end symbol
			 * @param istream input stream
			 * @param container_type container type to know the end symbol
			 * @return bool
			 */
			bool 											FindContainerEnd(std::istream& istream, const Container::Type& container_type);

			/**
			 * Consumes whitespace (and tabs, newline, etc) updating line number
			 * @param istream stream to consume whitespace from
			 */
			void 											ConsumeWS(std::istream& istream);

			/**
			 * Gets a string ignoring leading whitespace updating line number
			 * @param istream input stream
			 * @return string value
			 */
			std::string 									GetStringIgnoringWS(std::istream& istream);
	};
	/**
	 * Initializes configuration with istream (when istream is in the left part)
	 * @param istream input stream
	 * @param file Config to put data to
	 */
	STORMBYTE_CONFIG_PUBLIC Config&							operator>>(std::istream& istream, Config& file);

	/**
	 * Initializes configuration with string (when string is in the left part)
	 * @param str input string
	 * @param file Config to put data to
	 */
	STORMBYTE_CONFIG_PUBLIC Config&							operator>>(const std::string& str, Config& file);

	/**
	 * Output configuration serialized to output stream (when output stream is in the left part)
	 * @param ostream output stream
	 * @param file Config to get data from
	 */
	STORMBYTE_CONFIG_PUBLIC std::ostream&					operator<<(std::ostream& ostream, const Config& file);

	/**
	 * Output configuration serialized to string (when string is in the left part)
	 * @param str output string
	 * @param file Config to get data from
	 */
	STORMBYTE_CONFIG_PUBLIC std::string& 					operator<<(std::string& str, const Config& file);
}