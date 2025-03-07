#pragma once

#include <config/container/base.hxx>

/**
 * @namespace Container
 * @brief All the classes for handling containers of configuration items
 */
namespace StormByte::Config::Container {
	/**
	 * @class Group
	 * @brief Group in configuration item which can hold other items and also subgroups and sublists recursivelly separated by spaces
	 * @code
	 * settings = {
	 * 		username = "StormByte"
	 * 		credit = 66.5
	 * }
	 * @endcode
	 */
	class STORMBYTE_CONFIG_PUBLIC Group: public Base {
		public:
			/**
			 * Constructor
			 */
			constexpr Group():Base(Type::Group) {}

			/**
			 * Copy constructor
			 */
			constexpr Group(const Group&)					= default;

			/**
			 * Move constructor
			 */
			constexpr Group(Group&&) noexcept				= default;

			/**
			 * Assignment operator
			 */
			constexpr Group& operator=(const Group&)		= default;

			/**
			 * Move assignment operator
			 */
			constexpr Group& operator=(Group&&) noexcept	= default;

			/**
			 * Destructor
			 */
			constexpr ~Group() noexcept override			= default;

			/**
			 * Adds an item to the container
			 * @param item item to add
			 * @param on_existing action to take if item name already exists
			 * @throw ItemNameAlreadyExists if item name already exists
			 * @return reference to added item
			 */
			virtual Item&									Add(Item&& item, const OnExistingAction& on_existing = OnExistingAction::ThrowException) override;

			/**
			 * Adds an item to the container
			 * @param item item to add
			 * @param on_existing action to take if item name already exists
			 * @throw ItemNameAlreadyExists if item name already exists
			 * @return reference to added item
			 */
			virtual Item&									Add(const Item& item, const OnExistingAction& on_existing = OnExistingAction::ThrowException) override {
				Item copy(item);
				return Add(std::move(copy), on_existing);
			}

			/**
			 * Clones the container
			 * @return cloned container
			 */
			PointerType										Clone() const override;

			/**
			 * Moves the container
			 * @return moved container
			 */
			PointerType										Move() override;
	};
}