#pragma once

#include <config/comment/type.hxx>
#include <util/templates/clonable.hxx>

#include <string>

/**
 * @namespace Comment
 * @brief All the classes for handling comments on configuration files
 */
namespace StormByte::Config::Comment {
	/**
	 * @class Base
	 * @brief Abstract class for a comment item
	 */
	class STORMBYTE_CONFIG_PUBLIC Base:public Util::Templates::Clonable<Base> {
		public:
			/**
			 * Constructor
			 * @param comment comment string
			 */
			Base(const std::string& comment);

			/**
			 * Move Constructor
			 * @param comment comment string
			 */
			Base(std::string&& comment);

			/**
			 * Copy constructor
			 * @param base comment to copy
			 */
			Base(const Base& base)								= default;

			/**
			 * Move constructor
			 * @param base comment to move
			 */
			Base(Base&& base)									= default;

			/**
			 * Assignment operator
			 * @param base comment to copy
			 */
			Base& operator=(const Base& base)					= default;

			/**
			 * Move assignment operator
			 * @param base comment to move
			 */
			Base& operator=(Base&& base)						= default;

			/**
			 * Destructor
			 */
			virtual ~Base() noexcept							= default;

			/**
			 * Compares two comments
			 * @param other comment to compare to
			 * @return bool are they equal?
			 */
			constexpr bool 										operator==(const Base& other) const noexcept {
				return m_comment == other.m_comment;
			}

			/**
			 * Compares two comments
			 * @param other comment to compare to
			 * @return bool are they not equal?
			 */
			constexpr bool 										operator!=(const Base& other) const noexcept {
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
			constexpr Base& 									operator=(const std::string& value) {
				m_comment = value;
				return *this;
			}

			/**
			 * Moves the comment to a string
			 * @param value comment to set
			 * @return reference to comment
			 */
			constexpr Base& 									operator=(std::string&& value) {
				m_comment = std::move(value);
				return *this;
			}

			/**
			 * Appends a string to the comment
			 * @param value string to append
			 * @return reference to comment
			 */
			constexpr Base& 									operator+=(const std::string& value) {
				m_comment += value;
				return *this;
			}

			/**
			 * Moves a string to the comment
			 * @param value string to move
			 * @return reference to comment
			 */
			constexpr Base& 									operator+=(std::string&& value) {
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
				return Comment::TypeAsString(this->GetType());
			}

			/**
			 * Gets the comment string
			 * @return comment string
			 */
			constexpr virtual Type 								GetType() const noexcept = 0;

		protected:
			std::string 										m_comment;	///< Comment string
	};
}