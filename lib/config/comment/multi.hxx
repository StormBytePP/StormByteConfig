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
			constexpr MultiLine(const std::string& comment):Base(comment) {
				m_type = Type::MultiLine;
			}

			/**
			 * Move Constructor
			 * @param comment comment string
			 */
			constexpr MultiLine(std::string&& comment):Base(std::move(comment)) {
				m_type = Type::MultiLine;
			}

			/**
			 * Copy constructor
			 */
			constexpr MultiLine(const MultiLine&)						= default;

			/**
			 * Move constructor
			 */
			constexpr MultiLine(MultiLine&&)							= default;

			/**
			 * Assignment operator
			 */
			constexpr MultiLine& operator=(const MultiLine&)			= default;

			/**
			 * Move assignment operator
			 */
			constexpr MultiLine& operator=(MultiLine&&)					= default;

			/**
			 * Destructor
			 */
			constexpr ~MultiLine() noexcept override				= default;

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
	};
}