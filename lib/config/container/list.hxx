#pragma once

#include <config/container/base.hxx>

/**
 * @namespace Container
 * @brief All the classes for handling containers of configuration items
 */
namespace StormByte::Config::Container {
	/**
	 * @class List
	 * @brief List in configuration item which can hold other items and also subgroups and sublists recursivelly separated by spaces
	 * @code
	 * include_dirs = [
	 * 		"/usr/include"
	 * 		"/usr/local/include"
	 * ]
	 * @endcode
	 */
	class STORMBYTE_CONFIG_PUBLIC List final: public Base {
		public:
			/**
			 * Constructor
			 */
			List()									= default;

			/**
			 * Copy constructor
			 * @param list List to copy
			 */
			List(const List& list)					= default;

			/**
			 * Move constructor
			 * @param list List to move
			 */
			List(List&& list) noexcept				= default;

			/**
			 * Assignment operator
			 * @param list List to assign
			 */
			List& operator=(const List& list)		= default;

			/**
			 * Move assignment operator
			 * @param list List to move
			 */
			List& operator=(List&& list) noexcept	= default;

			/**
			 * Destructor
			 */
			~List() noexcept override				= default;

			/**
			 * Adds an item to the container
			 * @param item item to add
			 * @param on_existing action to take if item name already exists
			 * @throw ItemNameAlreadyExists if item name already exists
			 * @return reference to added item
			 */
			Item&									Add(Item&& item, const OnExistingAction& on_existing = OnExistingAction::ThrowException) override;

			/**
			 * Adds an item to the container
			 * @param item item to add
			 * @param on_existing action to take if item name already exists
			 * @throw ItemNameAlreadyExists if item name already exists
			 * @return reference to added item
			 */
			Item&									Add(const Item& item, const OnExistingAction& on_existing = OnExistingAction::ThrowException) override {
				Item copy(item);
				return Add(std::move(copy), on_existing);
			}

			/**
			 * Clones the list
			 * @return cloned list
			 */
			PointerType								Clone() const override;

			/**
			 * Moves the list
			 * @return moved list
			 */
			PointerType								Move() override;

			/**
			 * Gets the container type
			 * @return Container type
			 */
			constexpr Container::Type				GetType() const noexcept override {
				return Type::List;
			}
	};
}