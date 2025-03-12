#pragma once

#include <exception.hxx>
#include <config/item/type.hxx>

/**
 * @namespace Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	/**
	 * @class Exception
	 * @brief Base class for Config exceptions
	 */
	class STORMBYTE_CONFIG_PUBLIC Exception: public StormByte::Exception {
		public:
			/**
			 * Constructor
			 * @param reason
			 */
			Exception(const std::string& reason);

			/**
			 * Copy constructor
			 */
			Exception(const Exception&)					= default;

			/**
			 * Move constructor
			 */
			Exception(Exception&&) noexcept				= default;

			/**
			 * Assignment operator
			 */
			Exception& operator=(const Exception&)		= default;

			/**
			 * Move assignment operator
			 */
			Exception& operator=(Exception&&) noexcept	= default;

			/**
			 * Destructor
			 */
			~Exception() noexcept						= default;		
	};

	/**
	 * @class WrongValueTypeConversion
	 * @brief Exception thrown when value type requested is not correct
	 */
	class STORMBYTE_CONFIG_PUBLIC WrongValueTypeConversion final: public Exception {
		public:
			/**
			 * Constructor
			 * @param srctype item type trying to be converted
			 * @param dsttype target type
			 */
			WrongValueTypeConversion(const std::string& srctype, const std::string& dsttype);

			/**
			 * Copy constructor
			 */
			WrongValueTypeConversion(const WrongValueTypeConversion&)				= default;

			/**
			 * Move constructor
			 */
			WrongValueTypeConversion(WrongValueTypeConversion&&)					= default;

			/**
			 * Assignment operator
			 */
			WrongValueTypeConversion& operator=(const WrongValueTypeConversion&)	= default;

			/**
			 * Move assignment operator
			 */
			WrongValueTypeConversion& operator=(WrongValueTypeConversion&&)			= default;

			/**
			 * Destructor
			 */
			~WrongValueTypeConversion() noexcept override							= default;
	};

	/**
	 * @class ValueFailure
	 * @brief Exception thrown when setting a value to the wrong type
	 */
	class STORMBYTE_CONFIG_PUBLIC ValueFailure final: public Exception {
		public:
			/**
			 * Constructor
			 * @param srctype item source type
			 * @param dsttype destination type
			 */
			ValueFailure(const std::string& srctype, const std::string& dsttype);

			/**
			 * Copy constructor
			 */
			ValueFailure(const ValueFailure&)				= default;

			/**
			 * Move constructor
			 */
			ValueFailure(ValueFailure&&)					= default;

			/**
			 * Assignment operator
			 */
			ValueFailure& operator=(const ValueFailure&)	= default;

			/**
			 * Move assignment operator
			 */
			ValueFailure& operator=(ValueFailure&&)			= default;

			/**
			 * Destructor
			 */
			~ValueFailure() noexcept override				= default;
	};

	/**
	 * @class InvalidName
	 * @brief Exception thrown when adding to a group an item with invalid name
	 */
	class STORMBYTE_CONFIG_PUBLIC InvalidName final: public Exception {
		public:
			/**
			 * Constructor
			 */
			InvalidName();

			/**
			 * Constructor
			 * @param name name which was tried to use
			 * @param containertype container type
			 */
			InvalidName(const std::string& name, const Item::ContainerType& containertype);

			/**
			 * Copy operator
			 */
			InvalidName(const InvalidName&)				= default;

			/**
			 * Move operator
			 */
			InvalidName(InvalidName&&)					= default;

			/**
			 * Assignment operator
			 */
			InvalidName& operator=(const InvalidName&)	= default;

			/**
			 * Move assignment operator
			 */
			InvalidName& operator=(InvalidName&&)		= default;

			/**
			 * Destructor
			 */
			~InvalidName() noexcept override			= default;
	};

	/**
	 * @class InvalidPath
	 * @brief Exception thrown when adding to a group an item with invalid path
	 */
	class STORMBYTE_CONFIG_PUBLIC InvalidPath final: public Exception {
		public:
			/**
			 * Constructor
			 * @param name name which was tried to use
			 */
			InvalidPath(const std::string& name);

			/**
			 * Copy operator
			 */
			InvalidPath(const InvalidPath&)				= default;

			/**
			 * Move operator
			 */
			InvalidPath(InvalidPath&&)					= default;

			/**
			 * Assignment operator
			 */
			InvalidPath& operator=(const InvalidPath&)	= default;

			/**
			 * Move assignment operator
			 */
			InvalidPath& operator=(InvalidPath&&)		= default;

			/**
			 * Destructor
			 */
			~InvalidPath() noexcept override			= default;
	};

	/**
	 * @class ParseError
	 * @brief Thrown when a parser error is found when parsing File
	 */
	class STORMBYTE_CONFIG_PUBLIC ParseError final: public Exception {
		public:
			/**
			 * Constructor
			 * @param line the line where the error was found
			 * @param error the error string to show
			 */
			ParseError(const unsigned int& line, const std::string& error);

			/**
			 * Copy constructor
			 */
			ParseError(const ParseError&)				= default;

			/**
			 * Move constructor
			 */
			ParseError(ParseError&&)					= default;

			/**
			 * Assignment operator
			 */
			ParseError& operator=(const ParseError&)	= default;

			/**
			 * Move assignment operator
			 */
			ParseError& operator=(ParseError&&)			= default;

			/**
			 * Destructor
			 */
			~ParseError() noexcept override				= default;
	};

	/**
	 * @class ItemNotFound
	 * @brief Exception thrown lookup path fails
	 */
	class STORMBYTE_CONFIG_PUBLIC ItemNotFound final: public Exception {
		public:
			/**
			 * Constructor
			 * @param name item name which was not found
			 */
			ItemNotFound(const std::string& name);

			/**
			 * Copy constructor
			 */
			ItemNotFound(const ItemNotFound&)				= default;

			/**
			 * Move constructor
			 */
			ItemNotFound(ItemNotFound&&)					= default;

			/**
			 * Assignment operator
			 */
			ItemNotFound& operator=(const ItemNotFound&)	= default;

			/**
			 * Move assignment operator
			 */
			ItemNotFound& operator=(ItemNotFound&)			= default;

			/**
			 * Destructor
			 */
			~ItemNotFound() noexcept override				= default;
	};

	/**
	 * @class ItemAlreadyExists
	 * @brief Exception thrown when adding an item which already exists
	 */
	class STORMBYTE_CONFIG_PUBLIC ItemAlreadyExists final: public Exception {
		public:
			/**
			 * Constructor
			 */
			ItemAlreadyExists();

			/**
			 * Copy constructor
			 */
			ItemAlreadyExists(const ItemAlreadyExists&)				= default;

			/**
			 * Assignment operator
			 */
			ItemAlreadyExists& operator=(const ItemAlreadyExists&)	= default;

			/**
			 * Move assignment operator
			 */
			ItemAlreadyExists& operator=(ItemAlreadyExists&)		= default;

			/**
			 * Destructor
			 */
			virtual ~ItemAlreadyExists() noexcept					= default;
	};

	/**
	 * @class ItemNameAlreadyExists
	 * @brief Exception thrown when adding an item which name already exists
	 */
	class STORMBYTE_CONFIG_PUBLIC ItemNameAlreadyExists final: public Exception {
		public:
			/**
			 * Constructor
			 * @param name item name which was not found
			 */
			ItemNameAlreadyExists(const std::string& name);

			/**
			 * Copy constructor
			 */
			ItemNameAlreadyExists(const ItemNameAlreadyExists&)				= default;

			/**
			 * Assignment operator
			 */
			ItemNameAlreadyExists& operator=(const ItemNameAlreadyExists&)	= default;

			/**
			 * Move assignment operator
			 */
			ItemNameAlreadyExists& operator=(ItemNameAlreadyExists&)		= default;

			/**
			 * Destructor
			 */
			~ItemNameAlreadyExists() noexcept override						= default;
	};

	/**
	 * @class OutOfBounds
	 * @brief Exception thrown when index is out of bounds
	 */
	class STORMBYTE_CONFIG_PUBLIC OutOfBounds final: public Exception {
		public:
			/**
			 * Constructor
			 * @param index item index
			 * @param size size
			 */
			OutOfBounds(const size_t& index, const size_t& size);

			/**
			 * Copy constructor
			 */
			OutOfBounds(const OutOfBounds&)				= default;

			/**
			 * Move constructor
			 */
			OutOfBounds(OutOfBounds&&)					= default;

			/**
			 * Assignment operator
			 */
			OutOfBounds& operator=(const OutOfBounds&)	= default;

			/**
			 * Move assignment operator
			 */
			OutOfBounds& operator=(OutOfBounds&)		= default;

			/**
			 * Destructor
			 */
			~OutOfBounds() noexcept override			= default;
	};
}