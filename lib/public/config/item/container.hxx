#pragma once

#include <config/exception.hxx>
#include <config/item/base.hxx>
#include <config/type.hxx>

#include <queue>
#include <span>
#include <vector>

/**
 * @namespace Item
 * @brief All the configuration item classes namespace
 */
namespace StormByte::Config::Item {
	/**
	 * @class Container
	 * @brief Container class for a container of configuration items
	 * @tparam T container type
	 */
	class STORMBYTE_CONFIG_PUBLIC Container: public Base {
		public:
			/**
			 * Constructor
			 * @param type container type
			 */
			Container()											= default;

			/**
			 * Constructor
			 * @param name container name
			 */
			Container(const std::string& name);

			/**
			 * Constructor
			 * @param name container name
			 */
			Container(std::string&& name);

			/**
			 * Copy constructor
			 * @param base container to copy
			 */
			Container(const Container& base)					= default;

			/**
			 * Move constructor
			 * @param base container to move
			 */
			Container(Container&& base) noexcept				= default;

			/**
			 * Assignment operator
			 * @param base container to copy
			 */
			Container& operator=(const Container& base)			= default;

			/**
			 * Move assignment operator
			 * @param base container to move
			 */
			Container& operator=(Container&& base) noexcept		= default;

			/**
			 * Destructor
			 */
			virtual ~Container() noexcept override				= default;

			/**
			 * Gets a reference to Item by index
			 * @throw OutOfBounds if index is out of bounds
			 * @return size_t number of items
			 */
			Base& 												operator[](const size_t& index);

			/**
			 * Gets a const reference to Item by index
			 * @throw OutOfBounds if index is out of bounds
			 * @return size_t number of items
			 */
			const Base& 										operator[](const size_t& index) const;

			/**
			 * Gets a reference to Item by path
			 * @param path path to item
			 * @throw InvalidPath if path is invalid
			 * @throw ItemNotFound if item is not found
			 * @return Item& item
			 */
			Base& 												operator[](const std::string& path);

			/**
			 * Gets a const reference to Item by path
			 * @param path path to item
			 * @throw InvalidPath if path is invalid
			 * @throw ItemNotFound if item is not found
			 * @return Item& item
			 */
			inline const Base& 									operator[](const std::string& path) const {
				return LookUp(path);
			}

			/**
			 * Equality operator
			 * @param container container to compare
			 * @return is equal?
			 */
			bool 												operator==(const Container& container) const noexcept;

			/**
			 * Inequality operator
			 * @param container container to compare
			 * @return is not equal?
			 */
			inline bool 										operator!=(const Container& container) const noexcept {
				return !operator==(container);
			}

			/**
			 * Adds an item to the container (const reference)
			 * @param item item to add
			 * @param on_existing action to take if item name already exists
			 * @throw InvalidName if item name is not allowed
			 * @throw ItemNameAlreadyExists if item name already exists
			 * @return reference to added item
			 */
			inline Base& 										Add(const Base& item, const OnExistingAction& on_existing = OnExistingAction::ThrowException) {
				return Add(std::move(*item.Clone()), on_existing);
			}

			/**
			 * Adds an item to the container (rvalue reference)
			 * @param item item to add
			 * @param on_existing action to take if item name already exists
			 * @throw ItemNameAlreadyExists if item name already exists
			 * @return reference to added item
			 */
			inline Base& 										Add(Base&& item, const OnExistingAction& on_existing = OnExistingAction::ThrowException) {
				return Add(item.Move(), on_existing);
			}

			/**
			 * Adds an item to the container (pointer type, virtual pure)
			 * @param item item to add
			 * @param on_existing action to take if item name already exists
			 * @throw ItemNameAlreadyExists if item name already exists
			 * @return reference to added item
			 */
			Base& Add(Base::PointerType item, const OnExistingAction& on_existing);

			/**
			 * Clears all items
			 */
			inline void 										Clear() noexcept {
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
			void 												Remove(const size_t& index);

			/**
			 * Removes an item by path
			 * @param path path to item
			 * @throw InvalidPath if path is invalid
			 * @throw ItemNotFound if item is not found
			 */
			void												Remove(const std::string& path);

			/**
			 * Returns a string representation of the container
			 * @return string representation
			 */
			std::string 										Serialize(const int& indent_level) const noexcept override;

			/**
			 * Gets the start character for the container type
			 * @param type container type
			 * @return start character
			 */
			static constexpr std::pair<const char, const char>	EnclosureCharacters(const ContainerType& type) noexcept {
				switch(type) {
					case ContainerType::Group:	return std::make_pair<const char, const char>('{', '}');
					case ContainerType::List:	return std::make_pair<const char, const char>('[', ']');
					default:					return std::make_pair<const char, const char>('\0', '\0');
				}
			}

			/**
			 * Gets the end character for the container type
			 * @param type container type
			 * @return start character
			 */
			static constexpr const char							EndCharacter(const ContainerType& type) noexcept {
				switch(type) {
					case ContainerType::Group:	return '}';
					case ContainerType::List:	return ']';
					default:					return '\0';
				}
			}

			/**
			 * Get all items in the container
			 * @return std::span of items
			 */
			constexpr std::span<Base::PointerType> 				Items() noexcept {
				return std::span(m_items);
			}

			/**
			 * Get all items in the container
			 * @return std::span of items
			 */
			constexpr std::span<const Base::PointerType> 		Items() const noexcept {
				return std::span(m_items);
			}

			/**
			 * Gets the container type
			 * @return Container type
			 */
			constexpr virtual Item::ContainerType 				ContainerType() const noexcept = 0;

			/**
			 * Gets the container type as string
			 * @return Container type as string
			 */
			constexpr std::string 								ContainerTypeToString() const noexcept {
				return Item::TypeToString(this->ContainerType());
			}

			/**
			 * Gets the item type
			 * @return item Type
			 */
			constexpr Item::Type 								Type() const noexcept override {
				return Item::Type::Container;
			}

			/**
			 * Gets the number of items in the current level
			 * @return size_t number of items
			 */
			constexpr size_t 									Size() const noexcept {
				return m_items.size();
			}

			/**
			 * Gets the full number of items
			 * @return size_t number of items
			 */
			size_t 												Count() const noexcept;

		protected:
			std::vector<Base::PointerType> 						m_items;	///< Items in container

			/**
			 * Actions to be done before adding an item
			 * @param item item to check
			 * @param onexisting action to take if item name already exists
			 * @throw ItemNameAlreadyExists if item name already exists
			 */
			virtual Base::PointerType							BeforeAdditionActions(Base::PointerType item, const OnExistingAction onexisting) = 0;

		private:
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
			const Base& 										LookUp(const std::string& path) const;

			/**
			 * Looks up a child by path
			 * @param path path to child
			 * @throw ItemNotFound if not found
			 * @return const reference to found Item
			 */
			const Base& 										LookUp(std::queue<std::string>& path) const;

			/**
			 * Removes an item by path
			 * @param path path to item
			 * @throw ItemNotFound if item is not found
			 */
			void 												Remove(std::queue<std::string>& path);
	};
}