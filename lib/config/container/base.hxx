#pragma once

#include <config/container/type.hxx>
#include <config/comment/base.hxx>
#include <config/item.hxx>
#include <config/exception.hxx>
#include <util/string.hxx>

#include <span>

/**
 * @namespace Container
 * @brief All the classes for handling containers of configuration items
 */
namespace StormByte::Config::Container {
	/**
	 * @class Base
	 * @brief Base abstract class for a container of configuration items
	 */
	class STORMBYTE_CONFIG_PUBLIC Base: public Util::Templates::Clonable<Base> {
		public:
			/**
			 * @enum OnExistingAction
			 * @brief Action to take when a name is already in use
			 */
			enum class OnExistingAction: unsigned short {
				Keep,			///< Keep existing item
				Overwrite,		///< Overwrite existing item
				ThrowException	///< Throw exception
			};

			/**
			 * Copy constructor
			 * @param base container to copy
			 */
			Base(const Base& base)								= default;

			/**
			 * Move constructor
			 * @param base container to move
			 */
			Base(Base&& base) noexcept							= default;

			/**
			 * Assignment operator
			 * @param base container to copy
			 */
			Base& operator=(const Base& base)					= default;

			/**
			 * Move assignment operator
			 * @param base container to move
			 */
			Base& operator=(Base&& base)						= default;

			/**
			 * Destructor
			 */
			virtual ~Base() noexcept							= default;

			/**
			 * Gets a reference to Item by index
			 * @throw OutOfBounds if index is out of bounds
			 * @return size_t number of items
			 */
			virtual constexpr Item& 							operator[](const size_t& index) {
				return const_cast<Item&>(static_cast<const Base&>(*this)[index]);
			}

			/**
			 * Gets a const reference to Item by index
			 * @throw OutOfBounds if index is out of bounds
			 * @return size_t number of items
			 */
			virtual constexpr const Item& 						operator[](const size_t& index) const {
				if (index >= m_items.size())
					throw OutOfBounds(index, m_items.size());
				return m_items[index];
			}

			/**
			 * Gets a reference to Item by path
			 * @param path path to item
			 * @throw InvalidPath if path is invalid
			 * @throw ItemNotFound if item is not found
			 * @return Item& item
			 */
			constexpr virtual Item& 							operator[](const std::string& path) {
				return const_cast<Item&>(static_cast<const Base&>(*this)[path]);
			}

			/**
			 * Gets a const reference to Item by path
			 * @param path path to item
			 * @throw InvalidPath if path is invalid
			 * @throw ItemNotFound if item is not found
			 * @return Item& item
			 */
			inline virtual const Item& 							operator[](const std::string& path) const {
				return LookUp(path);
			}

			/**
			 * Equality operator
			 * @param other container to compare
			 * @return bool equal?
			 */
			constexpr bool 										operator==(const Base& other) const noexcept {
				return m_items == other.m_items;
			}

			/**
			 * Inequality operator
			 * @param other container to compare
			 * @return bool not equal?
			 */
			constexpr bool 										operator!=(const Base& other) const noexcept {
				return !(*this == other);
			}

			/**
			 * Adds an item to the container
			 * @param item item to add
			 * @param on_existing action to take if item name already exists
			 * @throw InvalidName if item name is not allowed
			 * @throw ItemNameAlreadyExists if item name already exists
			 * @return reference to added item
			 */
			virtual Item&										Add(const Item& item, const OnExistingAction& on_existing = OnExistingAction::ThrowException) = 0;

			/**
			 * Adds an item to the container
			 * @param item item to add
			 * @param on_existing action to take if item name already exists
			 * @throw ItemNameAlreadyExists if item name already exists
			 * @return reference to added item
			 */
			virtual Item&										Add(Item&& item, const OnExistingAction& on_existing = OnExistingAction::ThrowException) = 0;

			/**
			 * Gets a direct child by name
			 * @param name comment value
			 * @throw ItemNotFound if item is not found
			 * @return reference to found Item
			 */
			inline Item& 										Child(const std::string& name) {
				return const_cast<Item&>(static_cast<const Base&>(*this).Child(name));
			}

			/**
			 * Gets a direct child by name
			 * @param name comment value
			 * @throw ItemNotFound if item is not found
			 * @return const reference to found Item
			 */
			const Item& 										Child(const std::string& name) const;

			/**
			 * Clears all items
			 */
			constexpr void 										Clear() noexcept {
				m_items.clear();
			}

			/**
			 * Checks if item exists by path
			 * @param path path to item
			 * @return bool exists?
			 */
			bool 												Exists(const std::string& path) const;

			/**
			 * Removes an item by index
			 * @param index index to item
			 * @throw OutOfBounds if index is out of bounds
			 */
			constexpr void 										Remove(const size_t& index) {
				if (index >= m_items.size())
					throw OutOfBounds(index, m_items.size());
				m_items.erase(m_items.begin() + index);
			}

			/**
			 * Removes an item by path
			 * @param path path to item
			 * @throw InvalidPath if path is invalid
			 * @throw ItemNotFound if item is not found
			 */
			inline void											Remove(const std::string& path) {
				auto path_queue = Util::String::Explode(path, '/');
				Remove(path_queue);
			}

			/**
			 * Returns a string representation of the container
			 * @return string representation
			 */
			virtual std::string 								Serialize(const int& indent_level) const noexcept;

			/**
			 * Gets the start character for the container type
			 * @param type container type
			 * @return start character
			 */
			static constexpr std::pair<const char, const char>	EnclosureCharacters(const Type& type) noexcept {
				switch(type) {
					case Type::Group:	return std::make_pair<const char, const char>('{', '}');
					case Type::List:	return std::make_pair<const char, const char>('[', ']');
					default:			return std::make_pair<const char, const char>('\0', '\0');
				}
			}

			/**
			 * Gets the end character for the container type
			 * @param type container type
			 * @return start character
			 */
			static constexpr const char							EndCharacter(const Type& type) noexcept {
				switch(type) {
					case Type::Group:	return '}';
					case Type::List:	return ']';
					default:			return '\0';
				}
			}

			/**
			 * Get all items in the container
			 * @return std::span of items
			 */
			constexpr std::span<Item> 							Items() noexcept {
				return std::span(m_items);
			}

			/**
			 * Get all items in the container
			 * @return std::span of items
			 */
			constexpr std::span<const Item> 					Items() const noexcept {
				return std::span(m_items);
			}			

			/**
			 * Gets the container type
			 * @return Container type
			 */
			constexpr virtual Type 								GetType() const noexcept = 0;

			/**
			 * Gets the container type
			 * @return container type
			 */
			inline std::string 									TypeAsString() const noexcept {
				return Container::TypeAsString(this->GetType());
			}

			/**
			 * Gets the number of items in the current level
			 * @return size_t number of items
			 */
			constexpr virtual size_t 							Size() const noexcept {
				return m_items.size();
			}

			/**
			 * Gets the full number of items
			 * @return size_t number of items
			 */
			virtual size_t 										Count() const noexcept;

		protected:
			std::vector<Item> 									m_items;	///< Items in container

			/**
			 * Constructor
			 */
			Base()												= default;

			/**
			 * Internal function to get item contents as string
			 * @return item contents as std::string
			 */
			virtual std::string 								ContentsToString(const int& level) const noexcept;

			/**
			 * Internal function for checking if path name is valid
			 * @param name item path
			 * @return bool valid?
			 */
			static bool 										IsPathValid(const std::string& name) noexcept;

			/**
			 * Looks up a child by path
			 * @param path path to child
			 * @throw ItemNotFound if not found
			 * @return const reference to found Item
			 */
			inline const Item& 									LookUp(const std::string& path) const {
				auto path_queue = Util::String::Explode(path, '/');
				return LookUp(path_queue);
			}

			/**
			 * Looks up a child by path
			 * @param path path to child
			 * @throw ItemNotFound if not found
			 * @return const reference to found Item
			 */
			virtual const Item& 								LookUp(std::queue<std::string>& path) const;

			/**
			 * Removes an item by path
			 * @param path path to item
			 * @throw ItemNotFound if item is not found
			 */
			void 												Remove(std::queue<std::string>& path);
	};
}