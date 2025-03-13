#pragma once

#include <StormByte/config/alias.hxx>
#include <StormByte/config/item/value.hxx>
#include <StormByte/config/item/comment.hxx>
#include <StormByte/config/item/group.hxx>
#include <StormByte/config/item/list.hxx>
#include <StormByte/config/type.hxx>

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
	 * - SingleLineBash comment (starting with #)
	 * - MultiLineC comment (like C/C++ MultiLineC comments)
	 * - Group
	 * - List
	 */
	class STORMBYTE_CONFIG_PUBLIC Config {
		public:
			/**
			 * Constructor
			 */
			Config();

			/**
			 * Copy constructor
			 * @param config configuration to copy
			 */
			Config(const Config& config)							= default;

			/**
			 * Move constructor
			 * @param config configuration to move
			 */
			Config(Config&& config) noexcept						= default;

			/**
			 * Assignment operator
			 * @param config configuration to assign
			 */
			Config& operator=(const Config& config)					= default;

			/**
			 * Move assignment operator
			 * @param config configuration to move
			 */
			Config& operator=(Config&& config) noexcept				= default;

			/**
			 * Destructor
			 */
			virtual ~Config()										= default;

			/**
			 * Gets a reference to item by path
			 * @param path path to item
			 * @return item reference
			 */
			inline Item::Base&										operator[](const std::string& path) {
				return m_root.operator[](path);
			}

			/**
			 * Gets a const reference to item by path
			 * @param path path to item
			 * @return item const reference
			 */
			inline const Item::Base&								operator[](const std::string& path) const {
				return m_root.operator[](path);
			}

			/**
			 * Gets a reference to item by index
			 * @param index index of item
			 * @throw OutOfBounds if index is out of bounds
			 * @return item reference
			 */
			Item::Base& 											operator[](const size_t& index) {
				return m_root[index];
			}

			/**
			 * Gets a const reference to item by index
			 * @param index index of item
			 * @throw OutOfBounds if index is out of bounds
			 * @return item const reference
			 */
			const Item::Base& 										operator[](const size_t& index) const {
				return m_root[index];
			}

			/**
			 * Equality operator
			 * @param config configuration to compare
			 * @return is equal?
			 */
			inline bool												operator==(const Config& config) const noexcept {
				return m_root == config.m_root;
			}

			/**
			 * Inequality operator
			 * @param config configuration to compare
			 * @return is not equal?
			 */
			inline bool												operator!=(const Config& config) const noexcept {
				return !operator==(config);
			}

			/* INPUT */
			/**
			 * Take data from another configuration
			 * @param source source configuration to import
			 * @return Reference to configuration
			 */
			Config& 												operator<<(const Config& source);

			/**
			 * Initialize configuration with an input stream
			 * @param istream input stream
			 */
			void 													operator<<(std::istream& istream); // 1

			/**
			 * Initialize configuration with a string
			 * @param str input string
			 */
			void 													operator<<(const std::string& str); // 2

			/**
			 * Initializes configuration with istream (when istream is in the left part)
			 * @param istream input stream
			 * @param file Config to put data to
			 */
			friend STORMBYTE_CONFIG_PUBLIC Config&					operator>>(std::istream& istream, Config& file); // 3

			/**
			 * Initializes configuration with string (when string is in the left part)
			 * @param str input string
			 * @param file Config to put data to
			 */
			friend STORMBYTE_CONFIG_PUBLIC Config&					operator>>(const std::string& str, Config& file); // 4
			
			/* OUTPUT */
			/**
			 * Outut current configuration to another configuration
			 * @param dest configuration destination
			 * @return a reference to destination conf
			 */
			Config& 												operator>>(Config& dest) const;

			/**
			 * Output configuration serialized to output stream
			 * @param ostream output stream
			 */
			std::ostream&											operator>>(std::ostream& ostream) const; // 5

			/**
			 * Output configuration serialized to string
			 * @param str output string
			 */
			std::string&											operator>>(std::string& str) const; // 6

			/**
			 * Output configuration serialized to output stream (when output stream is in the left part)
			 * @param ostream output stream
			 * @param file Config to get data from
			 */
			friend STORMBYTE_CONFIG_PUBLIC std::ostream&			operator<<(std::ostream& ostream, const Config& file); // 7

			/**
			 * Output configuration serialized to string (when string is in the left part)
			 * @param str output string
			 * @param file Config to get data from
			 */
			friend STORMBYTE_CONFIG_PUBLIC std::string& 			operator<<(std::string&, const Config&); // 8

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
			inline Item::Base&										Add(const Item::Base& item) {
				return m_root.Add(item.Clone(), m_on_existing_action);
			}

			/**
			 * Adds an item to the container
			 * @param item item to add
			 * @throw ItemNameAlreadyExists if item name already exists
			 * @return reference to added item
			 */
			Item::Base&												Add(Item::Base&& item) {
				return m_root.Add(std::move(item.Move()), m_on_existing_action);
			}

			/**
			 * Clears all configuration items
			 */
			inline void												Clear() noexcept {
				m_root.Clear();
			}

			/**
			 * Checks if item exists by path
			 * @param path path to item
			 * @return bool exists?
			 */
			inline bool 											Exists(const std::string& path) const {
				return m_root.Exists(path);
			}

			/**
			 * Removes an item by path
			 * @param path item path
			 * @throw ItemNotFound if item is not found
			 */
			inline void												Remove(const std::string& path) {
				m_root.Remove(path);
			}

			/**
			 * Removes an item by position
			 * @param path item path
			 * @throw OutOfBounds if index is out of bounds
			 */
			inline void												Remove(const size_t& path) {
				m_root.Remove(path);
			}

			/**
			 * Sets a function to execute on name collission when inserting
			 * @param on_existing function to select element on collission
			 * @see Container::OnExistingAction
			 */
			constexpr void											OnExistingAction(const OnExistingAction& on_existing) {
				m_on_existing_action = on_existing;
			}

			/**
			 * Sets a function to execute on failure
			 * @param hook function to execute
			 */
			constexpr void											OnParseFailure(OnFailureHook hook) {
				m_on_parse_failure_hook = hook;
			}

			/**
			 * Adds a hook which will take Config as parameter and will be executed before read start
			 * Hooks will be executed *in order*
			 */
			constexpr void 											AddHookBeforeRead(HookFunction hook) {
				m_before_read_hooks.push_back(hook);
			}
			
			/**
			 * Adds a hook which will take Config as parameter and will be executed on successful read
			 * Hooks will be executed *in order*
			 */
			constexpr void 											AddHookAfterRead(HookFunction hook) {
				m_after_read_hooks.push_back(hook);
			}

			/**
			 * Gets the number of items in the current level
			 * @return size_t number of items
			 */
			constexpr virtual size_t								Size() const noexcept {
				return m_root.Size();
			}

			/**
			 * Gets the full number of items
			 * @return size_t number of items
			 */
			inline virtual size_t									Count() const noexcept {
				return m_root.Count();
			}

			/**
			 * Gets the items in the current level
			 * @return span of items
			 */
			constexpr std::span<Item::Base::PointerType>			Items() noexcept {
				return m_root.Items();
			}

			/**
			 * Gets the items in the current level
			 * @return span of items
			 */
			constexpr std::span<const Item::Base::PointerType> 		Items() const noexcept {
				return m_root.Items();
			}

		protected:
			Item::Group 											m_root;								///< Root group

			/**
			 * Ordered hook list which will be executed sequentially
			 * in their corresponding event
			 */
			HookFunctions 											m_before_read_hooks;				///< Hooks executed before reading
			HookFunctions											m_after_read_hooks;					///< Hooks executed after successful reading
			OptionalFailureHook										m_on_parse_failure_hook;			///< Hook executed on failure

			/**
			 * Function to override the default action when duplicate name is found when inserting
			 * Takes 3 parameters: current configuration, existing item and new item and will return
			 * the item to be inserted (or might throw to cancel the insert)
			 */
			StormByte::Config::OnExistingAction 					m_on_existing_action;				///< Action to take when item name already exists
	};
	/**
	 * Initializes configuration with istream (when istream is in the left part)
	 * @param istream input stream
	 * @param file Config to put data to
	 */
	STORMBYTE_CONFIG_PUBLIC Config&									operator>>(std::istream& istream, Config& file);

	/**
	 * Initializes configuration with string (when string is in the left part)
	 * @param str input string
	 * @param file Config to put data to
	 */
	STORMBYTE_CONFIG_PUBLIC Config&									operator>>(const std::string& str, Config& file);

	/**
	 * Output configuration serialized to output stream (when output stream is in the left part)
	 * @param ostream output stream
	 * @param file Config to get data from
	 */
	STORMBYTE_CONFIG_PUBLIC std::ostream&							operator<<(std::ostream& ostream, const Config& file);

	/**
	 * Output configuration serialized to string (when string is in the left part)
	 * @param str output string
	 * @param file Config to get data from
	 */
	STORMBYTE_CONFIG_PUBLIC std::string& 							operator<<(std::string& str, const Config& file);
}