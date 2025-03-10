#pragma once

#include <config/comment/base.hxx>

/**
 * @namespace Comment
 * @brief All the classes for handling comments on configuration files
 */
namespace StormByte::Config::Comment {
	/**
	 * @class MultiLine
	 * @brief Class for a multiline comment item in C/C++ style
	 */
	class STORMBYTE_CONFIG_PUBLIC MultiLine final: public Base {
		public:
			/**
			 * Constructor
			 * @param comment comment string
			 */
			MultiLine(const std::string& comment);

			/**
			 * Move Constructor
			 * @param comment comment string
			 */
			MultiLine(std::string&& comment);

			/**
			 * Copy constructor
			 * @param multi comment to copy
			 */
			MultiLine(const MultiLine& multi)					= default;

			/**
			 * Move constructor
			 * @param multi comment to move
			 */
			MultiLine(MultiLine&& multi)						= default;

			/**
			 * Assignment operator
			 * @param multi comment to copy
			 */
			MultiLine& operator=(const MultiLine& multi)		= default;

			/**
			 * Move assignment operator
			 * @param multi comment to move
			 */
			MultiLine& operator=(MultiLine&& multi)				= default;

			/**
			 * Destructor
			 */
			~MultiLine() noexcept override						= default;

			/**
			 * Gets the comment string
			 * @return comment string
			 */
			std::string 										Serialize(const int& indent_level) const noexcept override;

			/**
			 * Clones the object
			 * @return cloned object
			 */
			PointerType 										Clone() const override;

			/**
			 * Moves the object
			 * @return moved object
			 */
			PointerType 										Move() override;

			/**
			 * Gets the comment type
			 * @return comment type
			 */
			constexpr Comment::Type 							GetType() const noexcept override {
				return Type::MultiLine;
			}
	};
}