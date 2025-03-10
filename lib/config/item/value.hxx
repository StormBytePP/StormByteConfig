#pragma once

#include <config/item/base.hxx>

/**
 * @namespace Item
 * @brief All the configuration item classes namespace
 */
namespace StormByte::Config::Item {
	/**
	 * @class Value
	 * @brief Class for a item value in a configuration file
	 * @tparam T item type (only std::string, int, double, bool allowed)
	 */
	template<AllowedValueType T>
	class STORMBYTE_CONFIG_PUBLIC Value: public Base {
		public:
			/**
			 * Constructor
			 * @param value item value
			 */
			Value(const T& value):Base(), m_value(value) {}


			template <typename U = T>
			Value(const char* value) requires std::is_same_v<U, std::string>
				: Base(), m_value(std::string(value)) {}

			
			template <typename U = T>
			Value(const char* name, const char* value) requires std::is_same_v<U, std::string>
				: Base(std::string(name)), m_value(std::string(value)) {}

			/**
			 * Move Constructor
			 * @param value item value
			 */
			Value(T&& value):Base(), m_value(std::move(value)) {}

			/**
			 * Constructor
			 * @param name item name
			 * @param value item value
			 */
			Value(const std::string& name, const T& value):Base(name), m_value(value) {}

			/**
			 * Move Constructor
			 * @param name item name
			 * @param value item value
			 */
			Value(std::string&& name, T&& value):Base(std::move(name)), m_value(std::move(value)) {}

			/**
			 * Constructor overload (for std::string only)
			 * @param name item name
			 * @param value item value
			 */
			Value(const std::string& name, const char* value) requires std::is_same_v<T, std::string>
			: Base(name), m_value(std::string(value)) {}
	
			/**
			 * Move Constructor overload (for std::string only)
			 * @param name item name
			 * @param value item value
			 */
			Value(std::string&& name, const char* value) requires std::is_same_v<T, std::string>
			: Base(std::move(name)), m_value(std::string(value)) {}

			/**
			 * Copy constructor
			 * @param single item to copy
			 */
			Value(const Value& single)						= default;

			/**
			 * Move constructor
			 * @param single item to move
			 */
			Value(Value&& single) noexcept					= default;

			/**
			 * Assignment operator
			 * @param single item to copy
			 */
			Value& operator=(const Value& single)			= default;

			/**
			 * Move assignment operator
			 * @param single item to move
			 */
			Value& operator=(Value&& single) noexcept		= default;

			/**
			 * Destructor
			 */
			virtual ~Value() noexcept override				= default;

			/**
			 * Gets the item type
			 * @return item type
			 */
			constexpr virtual Item::Type 					Type() const noexcept override {
				if constexpr (std::is_same_v<T, std::string>) {
					return Item::Type::String;
				} else if constexpr (std::is_same_v<T, int>) {
					return Item::Type::Integer;
				} else if constexpr (std::is_same_v<T, double>) {
					return Item::Type::Double;
				} else if constexpr (std::is_same_v<T, bool>)
					return Item::Type::Bool;
			}

			/**
			 * Equality operator
			 * @param single item to compare
			 * @return is equal?
			 */
			bool											operator==(const Value<T>& single) const noexcept {
				// Compare the Base class
				if (Base::operator!=(single)) return false;
			
				// Compare the Type
				if constexpr (!std::is_same_v<T, decltype(m_value)>) {
					return false;
				}
			
				// Compare the m_value
				return m_value == single.m_value;
			}

			/**
			 * Inequality operator
			 * @param single item to compare
			 * @return is not equal?
			 */
			bool 											operator!=(const Value<T>& single) const noexcept {
				return !operator==(single);
			}

			/**
			 * Gets the item value
			 * @return item value
			 */
			T& 												operator*() noexcept {
				return m_value;
			}

			/**
			 * Gets the item value
			 * @return item value
			 */
			const T& 										operator*() const noexcept {
				return m_value;
			}

			/**
			 * Serializes the item
			 * @param indent_level intentation level
			 * @return serialized string
			 */
			std::string 									Serialize(const int& indent_level) const noexcept override;

			/**
			 * Clones the item
			 * @return cloned item
			 */
			virtual PointerType 							Clone() const override {
				return MakePointer<Value<T>>(*this);
			}

			/**
			 * Moves the item
			 * @return moved item
			 */
			virtual PointerType 							Move() override {
				return MakePointer<Value<T>>(std::move(*this));
			}

		protected:
			T m_value;										///< Item value
	};

	// Deduction guides
	Value(const char*) 				-> Value<std::string>;
	Value(const char*, const char*) -> Value<std::string>;
}