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
			Group()												= default;

			/**
			 * Copy constructor
			 * @param group group to copy
			 */
			Group(const Group& group)							= default;

			/**
			 * Move constructor
			 * @param group group to move
			 */
			Group(Group&& group) noexcept						= default;

			/**
			 * Assignment operator
			 * @param group group to copy
			 */
			constexpr Group& operator=(const Group& group)		= default;

			/**
			 * Move assignment operator
			 * @param group group to move
			 */
			constexpr Group& operator=(Group&& group) noexcept	= default;

			/**
			 * Destructor
			 */
			constexpr ~Group() noexcept override				= default;

			/**
			 * Adds an item to the container
			 * @param item item to add
			 * @param on_existing action to take if item name already exists
			 * @throw ItemNameAlreadyExists if item name already exists
			 * @return reference to added item
			 */
			virtual Item&										Add(Item&& item, const OnExistingAction& on_existing = OnExistingAction::ThrowException) override;

			/**
			 * Adds an item to the container
			 * @param item item to add
			 * @param on_existing action to take if item name already exists
			 * @throw ItemNameAlreadyExists if item name already exists
			 * @return reference to added item
			 */
			virtual Item&										Add(const Item& item, const OnExistingAction& on_existing = OnExistingAction::ThrowException) override {
				Item copy(item);
				return Add(std::move(copy), on_existing);
			}

			/**
			 * Clones the container
			 * @return cloned container
			 */
			PointerType											Clone() const override;

			/**
			 * Moves the container
			 * @return moved container
			 */
			PointerType											Move() override;

			/**
			 * Gets the container type
			 * @return Container type
			 */
			constexpr Container::Type							GetType() const noexcept override {
				return Type::Group;
			}
	};
}