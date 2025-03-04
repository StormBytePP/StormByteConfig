#pragma once

#include <config/visibility.h>
#include <util/templates/clonable.hxx>

#include <string>

/**
 * @namespace Comment
 * @brief All the classes for handling comments on configuration files
 */
namespace StormByte::Config::Comment {
	/**
	 * @class Comment
	 * @brief Abstract class for a comment item
	 */
	class STORMBYTE_CONFIG_PUBLIC Comment:public Util::Templates::Clonable<Comment> {
		public:
			/**
			 * @enum Type
			 * @brief Comment type
			 */
			enum class Type: unsigned short {
				SingleLine, ///< Single line comment starting with #
				MultiLine	///< Multi line comment starting with /* and ending with */
			};

			/**
			 * Gets strings from Type
			 * @param t type to convert
			 * @return string
			 */
			constexpr static std::string TypeAsString(const Type& t) noexcept {
				switch(t) {
					case Type::SingleLine: 	return "SingleLine";
					case Type::MultiLine: 	return "MultiLine";
					default: 				return "Unknown";
				}
			}

			/**
			 * Constructor
			 * @param comment comment string
			 */
			constexpr Comment(const std::string& comment):Util::Templates::Clonable<Comment>(), m_comment(comment) {}

			/**
			 * Move Constructor
			 * @param comment comment string
			 */
			constexpr Comment(std::string&& comment):Util::Templates::Clonable<Comment>(), m_comment(std::move(comment)) {};

			/**
			 * Copy constructor
			 */
			constexpr Comment(const Comment&)					= default;

			/**
			 * Move constructor
			 */
			constexpr Comment(Comment&&)						= default;

			/**
			 * Assignment operator
			 */
			constexpr Comment& operator=(const Comment&)		= default;

			/**
			 * Move assignment operator
			 */
			constexpr Comment& operator=(Comment&&)				= default;

			/**
			 * Destructor
			 */
			virtual constexpr ~Comment() noexcept				= default;

			/**
			 * Compares two comments
			 * @param other comment to compare to
			 * @return bool are they equal?
			 */
			constexpr bool 										operator==(const Comment& other) const noexcept {
				return m_comment == other.m_comment;
			}

			/**
			 * Compares two comments
			 * @param other comment to compare to
			 * @return bool are they not equal?
			 */
			constexpr bool 										operator!=(const Comment& other) const noexcept {
				return !(*this == other);
			}

			/**
			 * Gets the comment string
			 * @return comment string
			 */
			constexpr 											operator std::string&() {
				return m_comment;
			}

			/**
			 * Gets the comment string
			 * @return comment string
			 */
			constexpr 											operator std::string() const {
				return m_comment;
			}

			/**
			 * Sets the comment to a string
			 * @param value comment to set
			 * @return reference to comment
			 */
			constexpr Comment& 									operator=(const std::string& value) {
				m_comment = value;
				return *this;
			}

			/**
			 * Moves the comment to a string
			 * @param value comment to set
			 * @return reference to comment
			 */
			constexpr Comment& 									operator=(std::string&& value) {
				m_comment = std::move(value);
				return *this;
			}

			/**
			 * Appends a string to the comment
			 * @param value string to append
			 * @return reference to comment
			 */
			constexpr Comment& 									operator+=(const std::string& value) {
				m_comment += value;
				return *this;
			}

			/**
			 * Moves a string to the comment
			 * @param value string to move
			 * @return reference to comment
			 */
			constexpr Comment& 									operator+=(std::string&& value) {
				m_comment += std::move(value);
				return *this;
			}

			/**
			 * Appends a string to the comment
			 * @param value string to append
			 * @return new string
			 */
			constexpr std::string 								operator+(const std::string& value) const {
				return m_comment + value;
			}

			/**
			 * Moves a string to the comment
			 * @param value string to move
			 * @return new string
			 */
			constexpr std::string 								operator+(std::string&& value) const {
				return m_comment + std::move(value);
			}

			/**
			 * Serializes the comment item
			 * @param indent_level intentation level
			 * @return serialized string
			 */
			virtual std::string 								Serialize(const int& indent_level) const noexcept = 0;

			/**
			 * Gets the comment string
			 * @return comment string
			 */
			constexpr std::string 								TypeAsString() const noexcept {
				return TypeAsString(m_type);
			}

			/**
			 * Gets the comment string
			 * @return comment string
			 */
			constexpr const Type& 								GetType() const noexcept {
				return m_type;
			}

			/**
			 * Gets the number of items in the current level
			 * @return size_t number of items
			 */
			constexpr size_t 									Size() const noexcept {
				return 1;
			}

			/**
			 * Gets the full number of items
			 * @return size_t number of items
			 */
			constexpr size_t 									Count() const noexcept {
				return 1;
			}

		protected:
			Type 												m_type;		///< Comment type
			std::string 										m_comment;	///< Comment string
	};
}