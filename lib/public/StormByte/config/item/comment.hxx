#pragma once

#include <StormByte/config/item/value.hxx>

#include <string>

/**
 * @namespace Item
 * @brief All the configuration item classes namespace
 */
namespace StormByte::Config::Item {
	/**
	 * @class Comment
	 * @brief Class for a comment item
	 * @tparam T comment type
	 */
	template<CommentType T>
	class STORMBYTE_CONFIG_PUBLIC Comment final: public Value<std::string> {
		public:
			/**
			 * Constructor
			 * @param comment comment string
			 */
			Comment(const std::string& comment):Value<std::string>(comment) {}

			/**
			 * Move Constructor
			 * @param comment comment string
			 */
			Comment(std::string&& comment):Value<std::string>(std::move(comment)) {}

			/**
			 * Copy constructor
			 * @param base comment to copy
			 */
			Comment(const Comment& base)							= default;

			/**
			 * Move constructor
			 * @param base comment to move
			 */
			Comment(Comment&& base)									= default;

			/**
			 * Assignment operator
			 * @param base comment to copy
			 */
			Comment& operator=(const Comment& base)					= default;

			/**
			 * Move assignment operator
			 * @param base comment to move
			 */
			Comment& operator=(Comment&& base)						= default;

			/**
			 * Destructor
			 */
			~Comment() noexcept override							= default;

			/**
			 * Serializes the comment item
			 * @param indent_level intentation level
			 * @return serialized string
			 */
			std::string 											Serialize(const int& indent_level) const noexcept override;

			/**
			 * Gets the item type
			 * @return item type
			 */
			constexpr Item::Type									Type() const noexcept override {
				return Type::Comment;
			}

			/**
			 * Gets the comment string
			 * @return comment string
			 */
			constexpr CommentType 									CommentType() const noexcept {
				return T;
			}

			/**
			 * Converts comment type to string
			 * @return comment type string
			 */
			constexpr std::string									CommentTypeToString() const noexcept {
				return Item::TypeToString(T);
			}

			/**
			 * Clones the comment
			 * @return cloned comment
			 */
			PointerType												Clone() const override {
				return MakePointer<Comment<T>>(*this);
			}

			/**
			 * Moves the comment
			 * @return moved comment
			 */
			PointerType												Move() override {
				return MakePointer<Comment<T>>(std::move(*this));
			}
	};
}