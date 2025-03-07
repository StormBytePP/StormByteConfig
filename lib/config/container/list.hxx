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
			constexpr List():Base(Type::List) {}

			/**
			 * Copy constructor
			 */
			List(const List&)					= default;

			/**
			 * Move constructor
			 */
			List(List&&) noexcept				= default;

			/**
			 * Assignment operator
			 */
			List& operator=(const List&)		= default;

			/**
			 * Move assignment operator
			 */
			List& operator=(List&&) noexcept	= default;

			/**
			 * Destructor
			 */
			~List() noexcept override			= default;

			/**
			 * Adds an item to the container
			 * @param item item to add
			 * @param on_existing action to take if item name already exists
			 * @throw ItemNameAlreadyExists if item name already exists
			 * @return reference to added item
			 */
			Item&								Add(Item&& item, const OnExistingAction& on_existing = OnExistingAction::ThrowException) override;

			/**
			 * Adds an item to the container
			 * @param item item to add
			 * @param on_existing action to take if item name already exists
			 * @throw ItemNameAlreadyExists if item name already exists
			 * @return reference to added item
			 */
			Item&								Add(const Item& item, const OnExistingAction& on_existing = OnExistingAction::ThrowException) override {
				Item copy(item);
				return Add(std::move(copy), on_existing);
			}

			/**
			 * Clones the list
			 * @return cloned list
			 */
			PointerType							Clone() const override;

			/**
			 * Moves the list
			 * @return moved list
			 */
			PointerType							Move() override;
	};
}