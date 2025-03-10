#pragma once

#include <config/comment/base.hxx>

/**
 * @namespace Comment
 * @brief All the classes for handling comments on configuration files
 */
namespace StormByte::Config::Comment {
	/**
	 * @class SingleLine
	 * @brief Class for a single line comment item starting with #
	 */
	class STORMBYTE_CONFIG_PUBLIC SingleLine final: public Base {
		public:
			/**
			 * Constructor
			 * @param comment comment string
			 */
			SingleLine(const std::string& comment);

			/**
			 * Move Constructor
			 * @param comment comment string
			 */
			SingleLine(std::string&& comment);

			/**
			 * Copy constructor
			 * @param single comment to copy
			 */
			SingleLine(const SingleLine& single)						= default;

			/**
			 * Move constructor
			 * @param single comment to move
			 */
			constexpr SingleLine(SingleLine&& single)					= default;

			/**
			 * Assignment operator
			 * @param single comment to copy
			 */
			constexpr SingleLine& operator=(const SingleLine& single)	= default;

			/**
			 * Move assignment operator
			 * @param single comment to move
			 */
			constexpr SingleLine& operator=(SingleLine&& single)		= default;

			/**
			 * Destructor
			 */
			constexpr ~SingleLine() noexcept override					= default;

			/**
			 * Gets the comment string
			 * @return comment string
			 */
			std::string 												Serialize(const int& indent_level) const noexcept override;

			/**
			 * Clones the object
			 * @return cloned object
			 */
			PointerType													Clone() const override;

			/**
			 * Moves the object
			 * @return moved object
			 */
			PointerType													Move() override;

			/**
			 * Gets the comment type
			 * @return comment type
			 */
			constexpr Comment::Type										GetType() const noexcept override {
				return Type::SingleLine;
			}
	};
}