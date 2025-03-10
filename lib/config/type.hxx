#pragma once

/**
 * @namespace Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	/**
	 * @enum OnExistingAction
	 * @brief Action to take when a name is already in use
	 */
	enum class OnExistingAction: unsigned short {
		Keep,			///< Keep existing item
		Overwrite,		///< Overwrite existing item
		ThrowException	///< Throw exception
	};
}