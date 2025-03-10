#pragma once

#include <config/exception.hxx>
#include <config/item/type.hxx>
#include <util/templates/clonable.hxx>

#include <optional>
#include <string>
#include <type_traits>

namespace StormByte::Config::Item {
	// Forwards
	class Container;
	class Group;
	class List;
	template<AllowedValueType T> class Value;

	/**
	 * Checks if a name is valid
	 * @param name name to check
	 * @return is name valid?
	 */
	bool STORMBYTE_CONFIG_PUBLIC IsNameValid(const std::string&) noexcept;
	
	/**
	 * @class Base
	 * @brief Base class for a configuration item
	 */
	class STORMBYTE_CONFIG_PUBLIC Base: public Util::Templates::Clonable<Base, std::shared_ptr<Base>> {
		public:
			/**
			 * Constructor
			 * @param type item type
			 */
			Base() 											= default;

			/**
			 * Constructor
			 * @param name item name
			 */
			Base(const std::string& name);

			/**
			 * Copy constructor
			 * @param base item to copy
			 */
			Base(const Base& base) 							= default;
			
			/**
			 * Move constructor
			 * @param base item to move
			 */
			Base(Base&& base) noexcept						= default;

			/**
			 * Assignment operator
			 * @param base item to copy
			 */
			Base& operator=(const Base& base) 				= default;

			/**
			 * Move assignment operator
			 * @param base item to move
			 */
			Base& operator=(Base&& base) noexcept 			= default;

			/**
			 * Destructor
			 */
			virtual ~Base() noexcept 						= default;

			/**
			 * Equality operator
			 * @param base item to compare
			 * @return is equal?
			 */
			inline bool 									operator==(const Base& base) const noexcept {
				return m_name == base.m_name;
			}

			/**
			 * Inequality operator
			 * @param base item to compare
			 * @return is not equal?
			 */
			inline bool 									operator!=(const Base& base) const noexcept {
				return !operator==(base);
			}

			/**
			 * Gets the item name
			 * @return item name
			 */
			constexpr const std::optional<std::string>& 	Name() const noexcept {
				return m_name;
			}

			/**
			 * Sets the item name
			 * @return item name
			 */
			constexpr void 									Name(const std::string& name) noexcept {
				m_name = name;
			}

			/**
			 * Checks if current name is valid
			 * @return is name valid?
			 */
			inline bool 									IsNameValid() const noexcept {
				return m_name.has_value() && Item::IsNameValid(m_name.value());
			}

			/**
			 * Gets the item name
			 * @return item name
			 */
			constexpr virtual Type 							Type() const noexcept = 0;

			/**
			 * Gets the item type as string
			 * @return item type as string
			 */
			constexpr std::string							TypeToString() const noexcept {
				return Item::TypeToString(this->Type());
			}

			/**
			 * Gets the item name
			 * @return item name
			 */
			virtual std::string								Serialize(const int& indent_level) const noexcept;

			/**
			 * Converts current configuration to string
			 * @return configuration as string
			 */
			operator 										std::string() const {
				return this->Serialize(0);
			}

			/**
			 * Gets the item value
			 * @tparam T item value type
			 * @return item value
			 */
			template<typename T>
			const T& Value() const {
				if constexpr (std::is_base_of_v<std::remove_reference_t<decltype(*this)>, T>) {
					// Direct match: T (e.g., Container, Group, etc.) matches this object type
					return static_cast<const T&>(*this);
				} else if constexpr (std::is_base_of_v<std::remove_reference_t<decltype(*this)>, Item::Value<T>>) {
					// Indirect match: T is wrapped inside Item::Value<T>
					return *static_cast<const Item::Value<T>&>(*this);
				} else {
					// No valid match: Throw an exception for incorrect conversion
					throw WrongValueTypeConversion(this->TypeToString(), typeid(T).name());
				}
			}
			
			/**
			 * Gets the item value
			 * @tparam T item value type
			 * @return item value
			 */
			template<typename T>
			T& Value() {
				return const_cast<T&>(static_cast<const Base&>(*this).Value<T>());
			}
				

		protected:
			std::optional<std::string> m_name;				///< Item name
	};
}