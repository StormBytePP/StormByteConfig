#pragma once

#include <config/exception.hxx>
#include <config/alias.hxx>

#include <optional>

/**
 * @namespace Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	class Container;
	namespace Comment {
		class Comment;
	}
	/**
	 * @class Item
	 * @brief Class for a configuration item
	 */
	class STORMBYTE_CONFIG_PUBLIC Item {
		public:
			/**
			 * @enum Type
			 * @brief Item type
			 */
			enum class Type: unsigned short {
				String,		///< String item
				Integer,	///< Integer item
				Double,		///< Double item
				Comment,	///< Comment item
				Bool,		///< Bool item
				Container	///< Container item
			};
			/**
			 * Gets strings from Type
			 * @param t type to convert
			 * @return string
			 */
			static constexpr const char* TypeAsString(const Type& t) noexcept {
				switch(t) {
					case Type::String: 		return "String";
					case Type::Integer:		return "Integer";
					case Type::Double: 		return "Double";
					case Type::Comment:		return "Comment";
					case Type::Bool:		return "Bool";
					case Type::Container:	return "Container";
					default:				return "Unknown";
				}
			}
			/**
			 * Constructor for an empty item is not allowed
			 */
			Item()										= delete;

			/**
			 * Creates an item with a container value
			 * @param name item name
			 * @param value container value
			 */
			Item(const std::string& name, const Container& value);

			/**
			 * Creates an item with a group value
			 * @param value item value
			 */
			Item(const Container& value);

			/**
			 * Creates an item moving the group value
			 * @param name item name
			 * @param value item value
			 */
			Item(std::string&& name, Container&& value);

			/**
			 * Creates an item with a comment value
			 * @param value item value
			 */
			Item(const Comment::Comment& value);

			/**
			 * Creates an item with a comment value
			 * @param value item value
			 */
			Item(Comment::Comment&& value);

			/**
			 * Creates an item moving the group value
			 * @param value item value
			 */
			Item(Container&& value);

			/**
			 * Creates an item with a string value
			 * @param name item name
			 * @param value item value
			 */
			Item(const std::string& name, const std::string& value);

			/**
			 * Creates an item with a string value
			 * @param value item value
			 */
			Item(const std::string& value);

			/**
			 * Creates an item with a string value
			 * @param name item name
			 * @param value item value
			 */
			Item(std::string&& name, std::string&& value);

			/**
			 * Creates an item with a string value
			 * @param value item value
			 */
			Item(std::string&& value);

			/**
			 * Creates an item with a string value
			 * @param name item name
			 * @param value item value
			 */
			Item(const char* name, const char* value);

			/**
			 * Creates an item with a string value
			 * @param value item value
			 */
			Item(const char* value);

			/**
			 * Creates an item with a int value
			 * @param name item name
			 * @param value item value
			 */
			Item(const std::string& name, const int& value);

			/**
			 * Creates an item with a int value
			 * @param value item value
			 */
			Item(const int& value);

			/**
			 * Creates an item with a double value
			 * @param name item name
			 * @param value item value
			 */
			Item(const std::string& name, const double& value);

			/**
			 * Creates an item with a double value
			 * @param value item value
			 */
			explicit Item(const double& value);

			/**
			 * Creates an item with a bool value
			 * @param name item name
			 * @param value item value
			 */
			Item(const std::string& name, bool value);

			/**
			 * Creates an item with a bool value
			 * @param value item value
			 */
			explicit Item(bool value);

			/**
			 * Copy constructor
			 */
			Item(const Item&)							= default;

			/**
			 * Move constructor
			 */
			Item(Item&&) noexcept						= default;

			/**
			 * Assignment operator
			 */
			Item& operator=(const Item&)				= default;

			/**
			 * Move operator
			 */
			Item& operator=(Item&&) noexcept			= default;

			/**
			 * Destructor
			 */
			virtual ~Item() noexcept					= default;

			/**
			 * Equality operator
			 * @param other item to compare
			 * @return bool equal?
			 */
			constexpr bool 								operator==(const Item& other) const noexcept {
				return m_name == other.m_name && m_type == other.m_type && m_value == other.m_value;
			}

			/**
			 * Inequality operator
			 * @param other item to compare
			 * @return bool not equal?
			 */
			constexpr bool 								operator!=(const Item& other) const noexcept {
				return !(*this == other);
			}

			/**
			 * Static function to check if a name is valid (only contains numbers, letters and underscore)
			 * @param name name to check
			 * @return bool valid?
			 */
			static bool 								IsNameValid(const std::string& name) noexcept;
			
			/**
			 * Static function to check if a path is valid (only contains numbers, letters, underscore and slashes)
			 * @param path path to check
			 * @return bool valid?
			 */
			static bool									IsPathValid(const std::string& path) noexcept;

			/**
			 * Gets item name
			 * @return item name
			 */
			constexpr const std::optional<std::string>&	Name() const {
				return m_name;
			}

			/**
			 * Gets item name
			 * @return item name
			 */
			constexpr std::optional<std::string>&		Name() {
				return m_name;
			}

			/**
			 * Gets item type
			 * @return item Type
			 */
			constexpr const Type&						GetType() const noexcept {
				return m_type;
			}

			/**
			 * Gets item type as string
			 * @return item type string
			 */
			constexpr const std::string					TypeAsString() const noexcept {
				return TypeAsString(m_type);
			}
			
			/**
			 * Value getter as reference
			 * @return value reference
			 */
			constexpr ItemStorage&						Value() {
				return m_value;
			}

			/**
			 * Value getter as const reference
			 * @return value const reference
			 */
			constexpr const ItemStorage&				Value() const {
				return m_value;
			}

			/**
			 * Value getter as reference
			 * @return value reference
			 */
			template<typename T> T&						Value();

			/**
			 * Value getter as const reference
			 * @return value const reference
			 */
			template<typename T> const T&				Value() const;

			#ifdef MSVC
			/**
			 * Value getter as reference
			 * @return value reference
			 */
			template<> Container& Value<Container>();

			/**
			 * Value getter as const reference
			 * @return value const reference
			 */
			template<> const Container& Value<Container>() const;
			#endif

			/**
			 * Serializes the boolean item
			 * @param indent_level intentation level
			 * @return serialized string
			 */
			virtual inline std::string					Serialize(const int& indent_level) const noexcept {
				return ContentsToString(indent_level);
			}

			/**
			 * Gets the number of items in the current level
			 * @return size_t number of items
			 */
			virtual constexpr size_t					Size() const noexcept {
				return 1;
			}

			/**
			 * Gets the full number of items
			 * @return size_t number of items
			 */
			virtual size_t								Count() const noexcept;

		protected:
			/**
			 * Item optional name
			 */
			std::optional<std::string>					m_name;	//< Item name

			/**
			 * Item type
			 */
			Type										m_type;	//< Item type

			/**
			 * Internal value
			 */
			ItemStorage										m_value; //< Item value

			/**
			 * Internal function to get item contents as string
			 * @param level intentation level
			 * @return item contents as std::string
			 */
			std::string 								ContentsToString(const int& level) const noexcept;

			/**
			 * Internal function to get "name = " string
			 */
			constexpr std::string 						NameEqualSignString() const noexcept {
				std::string result = "";
				if (m_name)
					result += *m_name + " = ";
				return result;
			}
	};
}