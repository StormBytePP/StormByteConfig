#pragma once

#include <config/container.hxx>

/**
 * @namespace Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
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
	class STORMBYTE_CONFIG_PUBLIC List final: public Container {
		public:
			/**
			 * Constructor
			 */
			constexpr List():Container(Type::List) {}

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
			std::shared_ptr<Container>			Clone() const override;

			/**
			 * Moves the list
			 * @return moved list
			 */
			std::shared_ptr<Container>			Move() override;
	};
}