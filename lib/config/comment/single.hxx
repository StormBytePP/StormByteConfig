#pragma once

#include <config/comment.hxx>

/**
 * @namespace Comment
 * @brief All the classes for handling comments on configuration files
 */
namespace StormByte::Config::Comment {
	/**
	 * @class SingleLine
	 * @brief Class for a single line comment item starting with #
	 */
	class STORMBYTE_CONFIG_PUBLIC SingleLine final: public Comment {
		public:
			/**
			 * Constructor
			 * @param comment comment string
			 */
			constexpr SingleLine(const std::string& comment):Comment(comment) {
				m_type = Type::SingleLine;
			}

			/**
			 * Move Constructor
			 * @param comment comment string
			 */
			constexpr SingleLine(std::string&& comment):Comment(std::move(comment)) {
				m_type = Type::SingleLine;
			}

			/**
			 * Copy constructor
			 */
			constexpr SingleLine(const SingleLine&)						= default;

			/**
			 * Move constructor
			 */
			constexpr SingleLine(SingleLine&&)							= default;

			/**
			 * Assignment operator
			 */
			constexpr SingleLine& operator=(const SingleLine&)			= default;

			/**
			 * Move assignment operator
			 */
			constexpr SingleLine& operator=(SingleLine&&)				= default;

			/**
			 * Destructor
			 */
			constexpr ~SingleLine() noexcept override				= default;

			/**
			 * Gets the comment string
			 * @return comment string
			 */
			std::string 										Serialize(const int& indent_level) const noexcept override;

			/**
			 * Clones the object
			 * @return cloned object
			 */
			std::shared_ptr<Comment>							Clone() const override;

			/**
			 * Moves the object
			 * @return moved object
			 */
			std::shared_ptr<Comment>							Move() override;
	};
}