# StormByte

StormByte is a comprehensive, cross-platform C++ library aimed at easing system programming, configuration management, logging, and database handling tasks. This library provides a unified API that abstracts away the complexities and inconsistencies of different platforms (Windows, Linux).

## Features

- **System Operations**: Handles pipes, processes, and system variables seamlessly across different platforms.
- **Configuration Management**: Provides an intuitive API for reading and writing configuration files.
- **Logging**: Supports various logging levels and outputs, including file-based logging.
- **Database Handling**: Includes SQLite support for embedded database management while hiding SQLite3 internals conveniently.
- **Multimedia**: Includes a set of classes to work with multimedia files

## Table of Contents

- [Repository](#Repository)
- [Installation](#Installation)
- [Modules](#Modules)
	- [Base](https://dev.stormbyte.org/StormByte)
	- **Config**
	- [Database](https://dev.stormbyte.org/StormByte-Database)
	- [Logger](https://dev.stormbyte.org/StormByte-Logger)
	- [Multimedia](https://dev.stormbyte.org/StormByte-Multimedia)
	- [System](https://dev.stormbyte.org/StormByte-System)
- [Contributing](#Contributing)
- [License](#License)

## Repository

You can visit the code repository at [GitHub](https://github.com/StormBytePP/StormByte-Config)

## Installation

### Prerequisites

Ensure you have the following installed:

- C++23 compatible compiler
- CMake 3.12 or higher

### Building

To build the library, follow these steps:

```sh
git clone https://github.com/StormBytePP/StormByte-Config.git
cd StormByte-Config
mkdir build
cd build
cmake ..
make
```

## Modules

StormByte Library is composed by several modules:

### Config

#### Overview

The `Config` module of StormByte provides a flexible and human-readable way to manage configuration files. It supports initialization from any `std::istream`, setting pre and post read hooks using `std::function`, handling different data types (string, int, double, single and multiline comments, and containers such as lists and groups), and managing operation modes when items already exist before addition.

#### Initialization from `std::istream`

You can initialize the configuration from any `std::istream`, including `std::fstream`, `std::cin`, or even another `Config` object.

##### Example

```cpp
#include <StormByte/config/config.hxx>
#include <fstream>
#include <iostream>

using namespace StormByte::Config;

int main() {
    // Initialize from std::fstream
    Config config;
    std::ifstream file("config.cfg");
    file >> config;
    file.close();

    // Initialize from std::cin
    Config config2;
    std::cin >> config2;

    // Initialize from another Config object
    Config config3;
    config2 >> config3;

    return 0;
}
```

#### Hooks: Pre and Post Read

You can set pre and post read hooks using `std::function`. These hooks allow you to perform actions before and after reading the configuration, with the `Config` object passed as a reference argument.

##### Example

```cpp
#include <StormByte/config/config.hxx>
#include <iostream>
#include <functional>

using namespace StormByte::Config;

void pre_read_hook(Config& config) {
    std::cout << "Pre-read hook executed. Current config has " << config.Size() << " items." << std::endl;
}

void post_read_hook(Config& config) {
    std::cout << "Post-read hook executed. Current config has " << config.Size() << " items." << std::endl;
}

int main() {
    Config config;
    config.SetPreReadHook(pre_read_hook);
    config.SetPostReadHook(post_read_hook);

    // Read configuration (hooks will be called accordingly)
    std::ifstream file("config.cfg");
    file >> config;
    file.close();

    return 0;
}
```

#### Operation Modes for Existing Items

You can set the operation mode when an item already exists before adding a new one. Operation modes can include replace, ignore, or throw an exception (the default).

##### Example

```cpp
#include <StormByte/config/config.hxx>
#include <iostream>

using namespace StormByte::Config;

int main() {
    Config config;

    // Set operation mode to replace existing items
    config.SetOperationMode(Config::OperationMode::Replace);

    // Read configuration
    std::ifstream file("config.cfg");
    file >> config;
    file.close();

    return 0;
}
```

#### Data Types

The configuration supports various data types, including strings, integers, doubles, comments (single and multiline), and containers (lists and groups).

##### Strings

```plaintext
username = "example_user"
```

##### Example

```cpp
#include <StormByte/config/config.hxx>
#include <iostream>

using namespace StormByte::Config;

int main() {
    Config config;
    std::ifstream file("config.cfg");
    file >> config;
    file.close();

    const Item& username = config["username"];
    std::cout << "Username: " << username.Value<std::string>() << std::endl;

    return 0;
}
```

##### Integers

```plaintext
timeout = 30
```

##### Example

```cpp
#include <StormByte/config/config.hxx>
#include <iostream>

using namespace StormByte::Config;

int main() {
    Config config;
    std::ifstream file("config.cfg");
    file >> config;
    file.close();

    const Item& timeout = config["timeout"];
    std::cout << "Timeout: " << timeout.Value<int>() << std::endl;

    return 0;
}
```

##### Doubles

```plaintext
feature_timeout = 60.5
```

##### Example

```cpp
#include <StormByte/config/config.hxx>
#include <iostream>

using namespace StormByte::Config;

int main() {
    Config config;
    std::ifstream file("config.cfg");
    file >> config;
    file.close();

    const Item& feature_timeout = config["feature_timeout"];
    std::cout << "Feature Timeout: " << feature_timeout.Value<double>() << std::endl;

    return 0;
}
```

##### Comments

Single-line comments start with `#`, and multiline comments are enclosed between `/*` and `*/`.

```plaintext
# This is a single-line comment
/**
 * This is a multiline comment
 */
```

##### Example

```cpp
#include <StormByte/config/config.hxx>
#include <iostream>

using namespace StormByte::Config;

int main() {
    Config config;
    std::ifstream file("config.cfg");
    file >> config;
    file.close();

    // Retrieve comments using position
    for (std::size_t i = 0; i < config.Size(); ++i) {
        const Item& item = config[i];
        if (item.Type() == Item::Type::Comment) {
            std::cout << "Comment: " << item.Value<Comment>() << std::endl;
        }
    }

    return 0;
}
```

##### Containers: Lists

Lists are sequences of values enclosed in square brackets `[]` separated by spaces and can contain any other item (including nested lists and groups).

```plaintext
favorite_numbers = [3 14 42 "pi constant"]
```

##### Example

```cpp
#include <StormByte/config/config.hxx>
#include <iostream>

using namespace StormByte::Config;

int main() {
    Config config;
    std::ifstream file("config.cfg");
    file >> config;
    file.close();

    const List& favorite_numbers = config["favorite_numbers"].Value<List>();
    std::cout << "Favorite Numbers: ";
    for (const auto& number : favorite_numbers) {
	if (number.GetType() == Item::Type::Integer)
            std::cout << number.Value<int>() << " ";
        else
            std::cout << number.Value<std::string> << number;
    }
    std::cout << std::endl;

    return 0;
}
```

##### Containers: Groups

Groups are nested configurations that can contain other key-value pairs, groups, or lists.

```plaintext
settings = {
    username = "example_user"
    timeout = 30
}
```

##### Example

```cpp
#include <StormByte/config/config.hxx>
#include <iostream>

using namespace StormByte::Config;

int main() {
    Config config;
    std::ifstream file("config.cfg");
    file >> config;
    file.close();

    const Item& username = config["settings/username"];
    const Item& timeout = config["settings/timeout"];
    
    std::cout << "Username: " << username.Value<std::string>() << std::endl;
    std::cout << "Timeout: " << timeout.Value<int>() << std::endl;

    return 0;
}
```

This expanded section covers all requested features for the configuration file management in your library, with the correct handling and retrieval of comments. If there's anything specific you'd like to adjust or add, let me know!

## Contributing

Contributions are welcome! Please fork the repository and submit pull requests for any enhancements or bug fixes.

## License

This project is licensed under GPL v3 License - see the [LICENSE](LICENSE) file for details.
