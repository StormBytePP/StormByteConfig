#pragma once

#include <util/templates/variadic_value.hxx>

#include <memory>
#include <vector>

/**
 * @namespace Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	class Container;
	class Item;
	namespace Comment {
		class Comment;
	}

	using ContainerPTR 		= std::shared_ptr<Container>;																///< Shortcut alias for container pointer
	using CommentPTR 		= std::shared_ptr<Comment::Comment>;														///< Shortcut alias for comment pointer
	using ItemStorage 		= Util::Templates::VariadicValue<std::string, int, double, bool, ContainerPTR, CommentPTR>;	///< Shortcut alias for storage
	using ContainerStorage 	= std::vector<Item>;																		///< Shortcut alias for internal storage
}