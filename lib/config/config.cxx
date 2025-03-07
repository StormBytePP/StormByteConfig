#include <config/config.hxx>
#include <config/container/list.hxx>
#include <config/comment/multi.hxx>
#include <config/comment/single.hxx>

#include <regex>
#include <sstream>

using namespace StormByte::Config;

Config& Config::operator<<(const Config& source) {
	// We will not use serialize for performance reasons
	for (ConstIterator it = source.Begin(); it != source.End(); it++)
		Add(*it);
	return *this;
}

void Config::operator<<(std::istream& istream) { // 1
	StartParse(istream);
}

void Config::operator<<(const std::string& str) { // 2
	std::istringstream istream(str);
	*this << istream;
}

Config& StormByte::Config::operator>>(std::istream& istream, Config& config) { // 3
	config << istream;
	return config;
}

Config& StormByte::Config::operator>>(const std::string& str, Config& config) { // 4
	config << str;
	return config;
}

Config& Config::operator>>(Config& dest) const {
	dest << *this;
	return dest;
}

std::ostream& Config::operator>>(std::ostream& ostream) const { // 5
	ostream << (std::string)*this;
	return ostream;
}

std::string& Config::operator>>(std::string& str) const { // 6
	str += *this; // Conversion should be done automatically by operator std::string()
	return str;
}

std::ostream& StormByte::Config::operator<<(std::ostream& ostream, const Config& config) { // 7
	ostream << (std::string)config;
	return ostream;
}

std::string& operator<<(std::string& str, const Config& config) { // 8
	str += config;
	return str;
}

Config::operator std::string() const {
	std::string serialized = "";
	for (auto it = Begin(); it != End(); it++) {
		serialized += it->Serialize(0);// + "\n";
	}
	return serialized;
}

void Config::StartParse(std::istream& istream) {
	m_container_level = 0;
	m_current_line = 1;
	for (auto it = m_before_read_hooks.begin(); it != m_before_read_hooks.end(); it++) {
		(*it)(*this);
	}
	Parse(istream, *this, Parser::Mode::Named);
	for (auto it = m_after_read_hooks.begin(); it != m_after_read_hooks.end(); it++) {
		(*it)(*this);
	}
}

template<> Comment::MultiLine Config::ParseValue<Comment::MultiLine>(std::istream& istream) {
	bool comment_closed = false;
	char c;
	std::string buffer;
	while (!comment_closed && istream.get(c)) {
		switch(c) {
			case '*':
				if (istream.peek() == '/') {
					comment_closed = true;
					istream.get(c);
				}
				else
					buffer += c;
				break;
			case '\n':
				m_current_line++;
				[[fallthrough]];
			default:
				buffer += c;
				break;
		}
	}

	if (!comment_closed || istream.eof() || istream.fail())
		throw ParseError(m_current_line, "Unclosed multiline comment");

	return Comment::MultiLine(std::move(buffer));
}

template<> Comment::SingleLine Config::ParseValue<Comment::SingleLine>(std::istream& istream) {
	std::string line;
	std::getline(istream, line);
	m_current_line++;
	return Comment::SingleLine(std::move(line));
}

template<> double Config::ParseValue<double>(std::istream& istream) {
	const std::string buffer = GetStringIgnoringWS(istream);

	// std::stod just ignore extra characters so we better check
	std::regex double_regex(R"(^[+-]?(\d+(\.\d*)?|\.\d+)([eE][+-]?\d+)?$)");
	if (!std::regex_match(buffer, double_regex))
		throw ParseError(m_current_line, "Failed to parse double value '" + buffer + "'");
	try {
		double result = std::stod(buffer);
		return result;
	}
	catch (std::invalid_argument&) {
		throw ParseError(m_current_line, "Failed to parse double value near ");
	}
	catch (std::out_of_range&) {
		throw ParseError(m_current_line, "Double value " + buffer + " out of range");
	}
}

template<> int Config::ParseValue<int>(std::istream& istream) {
	const std::string buffer = GetStringIgnoringWS(istream);

	// stoi will ignore extra characters so we force check
	std::regex integer_regex(R"(^[+-]?\d+$)");
	if (!std::regex_match(buffer, integer_regex))
		throw ParseError(m_current_line, "Failed to parse integer value '" + buffer + '"');
	try {
		int result = std::stoi(buffer);
		return result;
	}
	catch (std::invalid_argument&) {
		throw ParseError(m_current_line, "Failed to parse integer value '" + buffer + '"');
	}
	catch (std::out_of_range&) {
		throw ParseError(m_current_line, "Integer value " + buffer + " out of range");
	}
}

template<> std::string Config::ParseValue<std::string>(std::istream& istream) {
	ConsumeWS(istream);
	std::string accumulator;
	// Guesser already detected the opened " so we skip it
	istream.seekg(1, std::ios::cur);
	bool string_closed = false;

	if (istream.eof())
		throw ParseError(m_current_line, "String content was expected but found EOF");

	// Do not skip space characters
	bool escape_next = false;
	char c;
	while (istream.get(c)) {
		if (escape_next) {
			// Handle escaped characters
			switch (c) {
				case '"':
				case '\\':
					accumulator += c;
					break;
				case 'n':
					accumulator += '\n';
					break;
				case 'r':
					accumulator += '\r';
					break;
				case 't':
					accumulator += '\t';
					break;
				default:
					throw ParseError(m_current_line, std::string("Invalid escape sequence: \\") + std::string(1, c));
					break;
			}
			escape_next = false;
		} else {
			switch(c) {
				case '\\':
					// Indicate that the next character should be escaped
					escape_next = true;
					break;
				case '"':
					string_closed = true;
					goto end;
				case '\n':
					m_current_line++;
					[[fallthrough]];
				default:
					accumulator += c;
					break;
			}
		}
	}

	end:
	if (!string_closed)
		throw ParseError(m_current_line, "Expected string closure but got EOF");

	return accumulator;
}

template<> bool Config::ParseValue<bool>(std::istream& istream) {
	const std::string buffer = GetStringIgnoringWS(istream);
	if (buffer != "true" && buffer != "false")
		throw ParseError(m_current_line, "Failed to parse boolean value '" + buffer + "'");
	return buffer == "true";
}

Parser::CommentType Config::FindComment(std::istream& istream) {
	ConsumeWS(istream);
	if (istream.eof())
		return Parser::CommentType::None;
	char c;
	auto current_position = istream.tellg();
	istream.get(c);
	if (c == '#') {
		return Parser::CommentType::SingleLine;
	}
	else if (c == '/') {
		istream.get(c);
		if (c == '*') {
			return Parser::CommentType::MultiLine;
		}
	}
	else {
		istream.clear();
		istream.seekg(current_position);
	}
	return Parser::CommentType::None;
}

void Config::FindAndParseComments(std::istream& istream, Container::Base& container) {
	Parser::CommentType type;
	while ((type = FindComment(istream)) != Parser::CommentType::None) {
		switch (type) {
			case Parser::CommentType::SingleLine:
				container.Add(Item(ParseValue<Comment::SingleLine>(istream)));
				break;
			case Parser::CommentType::MultiLine:
			container.Add(Item(ParseValue<Comment::MultiLine>(istream)));
				break;
			case Parser::CommentType::None:
				return;
		}
	}
}

std::unique_ptr<Item> Config::ParseItem(std::istream& istream, const Item::Type& type) {
	switch(type) {
		case Item::Type::Container:
			m_container_level++;
			switch (ParseContainerType(istream)) {
				case Container::Type::Group: {
					Container::Group group;
					Parse(istream, group, Parser::Mode::Named);
					return std::make_unique<Item>(std::move(group));
				}
				case Container::Type::List: {
					Container::List list;
					Parse(istream, list, Parser::Mode::Unnamed);
					return std::make_unique<Item>(std::move(list));
				}
			}
			break;
		case Item::Type::Comment: // Not handled here but make compiler not emit any warning
			return nullptr;
		case Item::Type::String:
			return std::make_unique<Item>(ParseValue<std::string>(istream));
		case Item::Type::Integer:
			return std::make_unique<Item>(ParseValue<int>(istream));
		case Item::Type::Double:
			return std::make_unique<Item>(ParseValue<double>(istream));
		case Item::Type::Bool:
			return std::make_unique<Item>(ParseValue<bool>(istream));
	}
	return nullptr;
}

void Config::Parse(std::istream& istream, Container::Base& container, const Parser::Mode& mode) {
	bool halt = false;
	FindAndParseComments(istream, container);
	while (!halt && !istream.eof()) {
		std::string item_name;

		if (mode == Parser::Mode::Named) {
			// Item Name
			item_name = ParseItemName(istream);
			
			// Equal expected
			std::string equal = GetStringIgnoringWS(istream);
			if (equal != "=") {
				throw ParseError(m_current_line, "Expected '=' after item name " + item_name + " but got " + equal);
			}
		}

		// Guessing type
		Item::Type type = ParseType(istream);
		std::unique_ptr<Item> item = ParseItem(istream, type);

		if (mode == Parser::Mode::Named)
			item->Name() = std::move(item_name);

		container.Add(std::move(*item), m_on_existing_action);

		FindAndParseComments(istream, container);

		if (FindContainerEnd(istream, container.GetType())) {
			// If it is encountered on level 0 it is a syntax error
			if (m_container_level == 0)
				throw ParseError(m_current_line, "Unexpected container end symbol");
			else
				m_container_level--;
			
			halt = true;
		}

		if (istream.fail()) {
			if (m_container_level > 0)
				throw ParseError(m_current_line, "Unexpected EOF");
			else
				halt = true;
		}
	}
}

std::string Config::ParseItemName(std::istream& istream) {
	const std::string name = GetStringIgnoringWS(istream);
	if (!Item::IsNameValid(name)) {
		throw ParseError(m_current_line, "Invalid item name: " + name);
	}
	return name;
}

Item::Type Config::ParseType(std::istream& istream) {
	/**** THIS FUNCTION ONLY DETECTS TYPE NOT CHECKS FOR VALIDITY *****/
	std::unique_ptr<Item::Type> type;
	ConsumeWS(istream);
	auto start_position = istream.tellg();
	std::string line;
	std::getline(istream, line);
	/** COMMENTS ARE NOT HANDLED HERE **/
	switch (line[0]) {
		case '"':
			type = std::make_unique<Item::Type>(Item::Type::String);
			break;
		case '[':
		case '{':
			type = std::make_unique<Item::Type>(Item::Type::Container);
			break;
		case '-':
		case '+':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': {
			// Possible int or double, we assume int until we find a "." (not exp notation as without a . it is allowed in integers too)
			// NOTE: We don't do sanity check here!
			type = std::make_unique<Item::Type>(Item::Type::Integer);
			bool halt = false;
			size_t pos = 1;
			while (pos < line.size() && !halt) {
				if (line[pos++] == '.') {
					type = std::make_unique<Item::Type>(Item::Type::Double);
					halt = true;
				}
			}
			break;
		}
		case 't':
		case 'f':
			type = std::make_unique<Item::Type>(Item::Type::Bool);
			break;
		default: {
			throw ParseError(m_current_line, "Unexpected " + std::string(1, line[0]) + " when parsing item type");
		}
	}
	istream.seekg(start_position);
	return *type;
}

StormByte::Config::Container::Type Config::ParseContainerType(std::istream& istream) {
	/**** THIS FUNCTION ONLY DETECTS TYPE NOT CHECKS FOR VALIDITY *****/
	char c = '\0';
	ConsumeWS(istream);
	istream.get(c);
	try {
		return Container::TypeFromStartCharacter(c);
	}
	catch (const StormByte::Exception&) {
		throw ParseError(m_current_line, "Unknown start character " + std::string(1, c) + " for container");
	}
}

bool Config::FindContainerEnd(std::istream& istream, const Container::Type& container_type) {
	/**** THIS FUNCTION ONLY DETECTS TYPE NOT CHECKS FOR VALIDITY *****/
	ConsumeWS(istream);
	char c;
	bool result = false;
	if (istream.get(c)) {
		if (EnclosureCharacters(container_type).second == c)
			result = true;
		else
			istream.unget();
	}
	return result;
}

void Config::ConsumeWS(std::istream& istream) {
	char c;
	while (istream.get(c)) {
		switch(c) {
			case '\n':
				m_current_line++;
				[[fallthrough]];
			case ' ':
			case '\t':
			case '\r':
				continue;
			default:
				istream.unget();
				return;
		}
	}
}

std::string Config::GetStringIgnoringWS(std::istream& istream) {
	ConsumeWS(istream);
	std::string buffer = "";
	bool character_found = false;
	char c;
	while (istream.get(c)) {
		switch(c) {
			case '\n':
				m_current_line++;
				[[fallthrough]];
			case ' ':
			case '\t':
			case '\r':
				if (character_found)
					return buffer;
				break;
			default:
				character_found = true;
				buffer += c;
				continue;
		}
	}
	return buffer;
}