#include <config/parser/parser.hxx>

#include <sstream>

using namespace StormByte::Config::Parser;

const std::regex Parser::c_double_regex(R"(^[+-]?\d+(\.\d+)?([eE][+-]?\d+)?$)");
const std::regex Parser::c_int_regex(R"(^[+-]?\d+$)");

Parser::Parser(const OnExistingAction& action):
m_container_level(0), m_current_line(1), c_on_existing_action(action) {}

StormByte::Config::Item::Group Parser::Parse(std::istream& istream, Item::Group& root, const OnExistingAction& action, const HookFunctions& before, const HookFunctions& after) {
	// Create parser
	Parser parser(action);

	// Execute before hooks
	for (const auto& hook: before)
		hook(root);
	parser.Parse(istream, root, Mode::Named);
	for (const auto& hook: after)
		hook(root);
	return root;
}

StormByte::Config::Item::Group Parser::Parse(const std::string& string, Item::Group& root, const OnExistingAction& action, const HookFunctions& before, const HookFunctions& after) {
	std::istringstream istream(string);
	return Parse(istream, root, action, before, after);
}

template<> StormByte::Config::Item::Comment<StormByte::Config::Item::CommentType::MultiLineC> Parser::ParseValue<StormByte::Config::Item::Comment<StormByte::Config::Item::CommentType::MultiLineC>>(std::istream& istream) {
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
		throw ParseError(m_current_line, "Unclosed MultiLineC comment");

	return Item::Comment<Item::CommentType::MultiLineC>(std::move(buffer));
}

template<> StormByte::Config::Item::Comment<StormByte::Config::Item::CommentType::SingleLineBash> Parser::ParseValue<StormByte::Config::Item::Comment<StormByte::Config::Item::CommentType::SingleLineBash>>(std::istream& istream) {
	std::string line;
	std::getline(istream, line);
	m_current_line++;
	return Item::Comment<Item::CommentType::SingleLineBash>(std::move(line));
}

template<> StormByte::Config::Item::Comment<StormByte::Config::Item::CommentType::SingleLineC> Parser::ParseValue<StormByte::Config::Item::Comment<StormByte::Config::Item::CommentType::SingleLineC>>(std::istream& istream) {
	std::string line;
	std::getline(istream, line);
	m_current_line++;
	return Item::Comment<Item::CommentType::SingleLineC>(std::move(line));
}

template<> double Parser::ParseValue<double>(std::istream& istream) {
	const std::string buffer = GetStringIgnoringWS(istream);

	// std::stod just ignore extra characters so we better check
	if (!std::regex_match(buffer, c_double_regex))
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

template<> int Parser::ParseValue<int>(std::istream& istream) {
	const std::string buffer = GetStringIgnoringWS(istream);

	// stoi will ignore extra characters so we force check
	if (!std::regex_match(buffer, c_int_regex))
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

template<> std::string Parser::ParseValue<std::string>(std::istream& istream) {
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

template<> bool Parser::ParseValue<bool>(std::istream& istream) {
	const std::string buffer = GetStringIgnoringWS(istream);
	if (buffer != "true" && buffer != "false")
		throw ParseError(m_current_line, "Failed to parse boolean value '" + buffer + "'");
	return buffer == "true";
}

CommentType Parser::FindComment(std::istream& istream) {
	ConsumeWS(istream);
	if (istream.eof())
		return CommentType::None;
	char c;
	auto current_position = istream.tellg();
	istream.get(c);
	if (c == '#') {
		return CommentType::SingleLineBash;
	}
	else if (c == '/') {
		istream.get(c);
		if (c == '*') {
			return CommentType::MultiLineC;
		}
		else if (c == '/') {
			return CommentType::SingleLineC;
		}
	}
	else {
		istream.clear();
		istream.seekg(current_position);
	}
	return CommentType::None;
}

void Parser::FindAndParseComments(std::istream& istream, Item::Container& container) {
	CommentType type;
	while ((type = FindComment(istream)) != CommentType::None) {
		switch (type) {
			case CommentType::SingleLineBash:
				container.Add(ParseValue<Item::Comment<Item::CommentType::SingleLineBash>>(istream));
				break;
				case CommentType::SingleLineC:
				container.Add(ParseValue<Item::Comment<Item::CommentType::SingleLineC>>(istream));
				break;
			case CommentType::MultiLineC:
				container.Add(ParseValue<Item::Comment<Item::CommentType::MultiLineC>>(istream));
				break;
			case CommentType::None:
				return;
		}
	}
}

StormByte::Config::Item::Base::PointerType Parser::ParseItem(std::istream& istream, const Item::Type& type) {
	switch(type) {
		case Item::Type::Container:
			m_container_level++;
			switch (ParseContainerType(istream)) {
				case Item::ContainerType::Group: {
					Item::Group group;
					Parse(istream, group, Mode::Named);
					return group.Move();
				}
				case Item::ContainerType::List: {
					Item::List list;
					Parse(istream, list, Mode::Unnamed);
					return list.Move();
				}
			}
			break;
		case Item::Type::Comment: // Not handled here but make compiler not emit any warning
			return nullptr;
		case Item::Type::String:
			return Item::Base::MakePointer<Item::Value<std::string>>(ParseValue<std::string>(istream));
		case Item::Type::Integer:
			return Item::Base::MakePointer<Item::Value<int>>(ParseValue<int>(istream));
		case Item::Type::Double:
			return Item::Base::MakePointer<Item::Value<double>>(ParseValue<double>(istream));
		case Item::Type::Bool:
			return Item::Base::MakePointer<Item::Value<bool>>(ParseValue<bool>(istream));
	}
	return nullptr;
}

void Parser::Parse(std::istream& istream, Item::Container& container, const Mode& mode) {
	bool halt = false;
	FindAndParseComments(istream, container);
	while (!halt && !istream.eof()) {
		std::string item_name;

		if (mode == Mode::Named) {
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
		auto item = ParseItem(istream, type);

		if (mode == Mode::Named)
			item->Name(std::move(item_name));

		container.Add(item, c_on_existing_action);

		FindAndParseComments(istream, container);

		if (FindContainerEnd(istream, container.ContainerType())) {
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

std::string Parser::ParseItemName(std::istream& istream) {
	const std::string name = GetStringIgnoringWS(istream);
	if (!Item::IsNameValid(name)) {
		throw ParseError(m_current_line, "Invalid item name: " + name);
	}
	return name;
}

StormByte::Config::Item::Type Parser::ParseType(std::istream& istream) {
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

StormByte::Config::Item::ContainerType Parser::ParseContainerType(std::istream& istream) {
	/**** THIS FUNCTION ONLY DETECTS TYPE NOT CHECKS FOR VALIDITY *****/
	char c = '\0';
	ConsumeWS(istream);
	istream.get(c);
	try {
		return Item::TypeFromStartCharacter(c);
	}
	catch (const StormByte::Exception&) {
		throw ParseError(m_current_line, "Unknown start character " + std::string(1, c) + " for container");
	}
}

bool Parser::FindContainerEnd(std::istream& istream, const Item::ContainerType& container_type) {
	/**** THIS FUNCTION ONLY DETECTS TYPE NOT CHECKS FOR VALIDITY *****/
	ConsumeWS(istream);
	char c;
	bool result = false;
	if (istream.get(c)) {
		if (Item::Container::EnclosureCharacters(container_type).second == c)
			result = true;
		else
			istream.unget();
	}
	return result;
}

void Parser::ConsumeWS(std::istream& istream) {
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

std::string Parser::GetStringIgnoringWS(std::istream& istream) {
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

namespace StormByte::Config::Parser {
	StormByte::Config::Item::Group Parse(std::istream& stream, Item::Group& root, const StormByte::Config::OnExistingAction& action, const HookFunctions& before, const HookFunctions& after) {
		return Parser::Parse(stream, root, action, before, after);
	}

	StormByte::Config::Item::Group Parse(const std::string& string, Item::Group& root, const StormByte::Config::OnExistingAction& action, const HookFunctions& before, const HookFunctions& after) {
		return Parser::Parse(string, root, action, before, after);
	}
}