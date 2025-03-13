#include <StormByte/config/parser/parser.hxx>

#include <sstream>

using namespace StormByte::Config::Parser;

const std::regex Parser::c_double_regex(R"(^[+-]?\d+(\.\d+)?([eE][+-]?\d+)?$)");
const std::regex Parser::c_int_regex(R"(^[+-]?\d+$)");

Parser::Parser(const OnExistingAction& action):
m_container_level(0), m_current_line(1), c_on_existing_action(action) {}

StormByte::Expected<void, StormByte::Config::ParseError> Parser::Parse(std::istream& istream, Item::Group& root, const OnExistingAction& action, const HookFunctions& before, const HookFunctions& after, const OptionalFailureHook& on_failure) {
	// Create parser
	Parser parser(action);

	// Execute before hooks
	for (const auto& hook: before)
		hook(root);
	auto res = parser.Parse(istream, root, Mode::Named);
	
	if (!res) {
		bool should_throw = true;
		if (on_failure)
			should_throw = (*on_failure)(root);

		if (should_throw)
			return Unexpected(std::move(res.error()));
		else
			return {};
	}

	for (const auto& hook: after)
		hook(root);
	return {};
}

StormByte::Expected<void, StormByte::Config::ParseError> Parser::Parse(const std::string& string, Item::Group& root, const OnExistingAction& action, const HookFunctions& before, const HookFunctions& after, const OptionalFailureHook& on_failure) {
	std::istringstream istream(string);
	return Parse(istream, root, action, before, after, on_failure);
}

template<> StormByte::Expected<StormByte::Config::Item::Comment<StormByte::Config::Item::CommentType::MultiLineC>, StormByte::Config::ParseError> Parser::ParseValue<StormByte::Config::Item::Comment<StormByte::Config::Item::CommentType::MultiLineC>>(std::istream& istream) {
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
		return Unexpected<ParseError>(m_current_line, "Unclosed MultiLineC comment");

	return Item::Comment<Item::CommentType::MultiLineC>(std::move(buffer));
}

template<> StormByte::Expected<StormByte::Config::Item::Comment<StormByte::Config::Item::CommentType::SingleLineBash>, StormByte::Config::ParseError> Parser::ParseValue<StormByte::Config::Item::Comment<StormByte::Config::Item::CommentType::SingleLineBash>>(std::istream& istream) {
	std::string line;
	std::getline(istream, line);
	m_current_line++;
	return Item::Comment<Item::CommentType::SingleLineBash>(std::move(line));
}

template<> StormByte::Expected<StormByte::Config::Item::Comment<StormByte::Config::Item::CommentType::SingleLineC>, StormByte::Config::ParseError> Parser::ParseValue<StormByte::Config::Item::Comment<StormByte::Config::Item::CommentType::SingleLineC>>(std::istream& istream) {
	std::string line;
	std::getline(istream, line);
	m_current_line++;
	return Item::Comment<Item::CommentType::SingleLineC>(std::move(line));
}

template<> StormByte::Expected<double, StormByte::Config::ParseError> Parser::ParseValue<double>(std::istream& istream) {
	const std::string buffer = GetStringIgnoringWS(istream);

	// std::stod just ignore extra characters so we better check
	if (!std::regex_match(buffer, c_double_regex))
		return Unexpected<ParseError>(m_current_line, "Failed to parse double value '" + buffer + "'");
	try {
		double result = std::stod(buffer);
		return result;
	}
	catch (std::invalid_argument&) {
		return Unexpected<ParseError>(m_current_line, "Failed to parse double value");
	}
	catch (std::out_of_range&) {
		return Unexpected<ParseError>(m_current_line, "Double value " + buffer + " out of range");
	}
}

template<> StormByte::Expected<int, StormByte::Config::ParseError> Parser::ParseValue<int>(std::istream& istream) {
	const std::string buffer = GetStringIgnoringWS(istream);

	// stoi will ignore extra characters so we force check
	if (!std::regex_match(buffer, c_int_regex))
		return Unexpected<ParseError>(m_current_line, "Failed to parse integer value '" + buffer + '"');
	try {
		int result = std::stoi(buffer);
		return result;
	}
	catch (std::invalid_argument&) {
		return Unexpected<ParseError>(m_current_line, "Failed to parse integer value '" + buffer + '"');
	}
	catch (std::out_of_range&) {
		return Unexpected<ParseError>(m_current_line, "Integer value " + buffer + " out of range");
	}
}

template<> StormByte::Expected<std::string, StormByte::Config::ParseError> Parser::ParseValue<std::string>(std::istream& istream) {
	ConsumeWS(istream);
	std::string accumulator;
	// Guesser already detected the opened " so we skip it
	istream.seekg(1, std::ios::cur);
	bool string_closed = false;

	if (istream.eof())
		return Unexpected<ParseError>(m_current_line, "String content was expected but found EOF");

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
					return Unexpected<ParseError>(m_current_line, std::string("Invalid escape sequence: \\") + std::string(1, c));
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
		return Unexpected<ParseError>(m_current_line, "Expected string closure but got EOF");

	return accumulator;
}

template<> StormByte::Expected<bool, StormByte::Config::ParseError> Parser::ParseValue<bool>(std::istream& istream) {
	const std::string buffer = GetStringIgnoringWS(istream);
	if (buffer != "true" && buffer != "false")
		return Unexpected<ParseError>(m_current_line, "Failed to parse boolean value '" + buffer + "'");
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

StormByte::Expected<void, StormByte::Config::ParseError> Parser::FindAndParseComments(std::istream& istream, Item::Container& container) {
	CommentType type;
	while ((type = FindComment(istream)) != CommentType::None) {
		switch (type) {
			case CommentType::SingleLineBash: {
				auto res = ParseValue<Item::Comment<Item::CommentType::SingleLineBash>>(istream);
				if (res)
					container.Add(std::move(res.value()));
				else
					return Unexpected(std::move(res.error()));
				break;
			}
			case CommentType::SingleLineC: {
				auto res = ParseValue<Item::Comment<Item::CommentType::SingleLineC>>(istream);
				if (res)
					container.Add(std::move(res.value()));
				else
					return Unexpected(std::move(res.error()));
				break;
			}
			case CommentType::MultiLineC: {
				auto res = ParseValue<Item::Comment<Item::CommentType::MultiLineC>>(istream);
				if (res)
					container.Add(std::move(res.value()));
				else
					return Unexpected(std::move(res.error()));
				break;
			}
			case CommentType::None:
				return {};
		}
	}
	return {};
}

StormByte::Expected<StormByte::Config::Item::Base::PointerType, StormByte::Config::ParseError> Parser::ParseItem(std::istream& istream, const Item::Type& type) {
	switch(type) {
		case Item::Type::Container: {
			m_container_level++;
			auto container_type = ParseContainerType(istream);
			if (container_type) {
				switch (container_type.value()) {
					case Item::ContainerType::Group: {
						Item::Group group;
						auto res = Parse(istream, group, Mode::Named);
						if (!res)
							return Unexpected(std::move(res.error()));
						return group.Move();
					}
					case Item::ContainerType::List: {
						Item::List list;
						auto res = Parse(istream, list, Mode::Unnamed);
						if (!res)
							return Unexpected(std::move(res.error()));
						return list.Move();
					}
					default:
						return Unexpected<ParseError>(m_current_line, "Unknown container type");
				}
			}
			else
				return Unexpected(std::move(container_type.error()));
		}
		case Item::Type::Comment: // Not handled here but make compiler not emit any warning
			return nullptr;
		case Item::Type::String: {
			auto res = ParseValue<std::string>(istream);
			if (res)
				return Item::Base::MakePointer<Item::Value<std::string>>(std::move(res.value()));
			else
				return Unexpected(std::move(res.error()));
		}
		case Item::Type::Integer: {
			auto res = ParseValue<int>(istream);
			if (res)
				return Item::Base::MakePointer<Item::Value<int>>(std::move(res.value()));
			else
				return Unexpected(std::move(res.error()));
		}
		case Item::Type::Double: {
			auto res = ParseValue<double>(istream);
			if (res)
				return Item::Base::MakePointer<Item::Value<double>>(std::move(res.value()));
			else
				return Unexpected(std::move(res.error()));
		}
		case Item::Type::Bool: {
			auto res = ParseValue<bool>(istream);
			if (res)
				return Item::Base::MakePointer<Item::Value<bool>>(std::move(res.value()));
			else
				return Unexpected(std::move(res.error()));
		}
		default:
			return Unexpected<ParseError>(m_current_line, "Unknown item type");
	}
}

StormByte::Expected<void, StormByte::Config::ParseError> Parser::Parse(std::istream& istream, Item::Container& container, const Mode& mode) {
	bool halt = false;
	auto res = FindAndParseComments(istream, container);
	if (!res)
		return Unexpected(std::move(res.error()));
	while (!halt && !istream.eof()) {
		std::string item_name;

		if (mode == Mode::Named) {
			// Item Name
			auto res = ParseItemName(istream);
			if (res)
				item_name = std::move(res.value());
			else
				return Unexpected(std::move(res.error()));
			
			// Equal expected
			std::string equal = GetStringIgnoringWS(istream);
			if (equal != "=") {
				return Unexpected<ParseError>(m_current_line, "Expected '=' after item name " + item_name + " but got " + equal);
			}
		}

		// Guessing type
		auto type_res = ParseType(istream);
		if (!type_res)
			return Unexpected(std::move(type_res.error()));
		Item::Type type = type_res.value();

		auto item_res = ParseItem(istream, type);
		if (!item_res)
			return Unexpected(std::move(item_res.error()));

		auto item = std::move(item_res.value());

		if (mode == Mode::Named)
			item->Name(std::move(item_name));

		container.Add(item, c_on_existing_action);

		res = FindAndParseComments(istream, container);
		if (!res)
			return Unexpected(std::move(res.error()));

		if (FindContainerEnd(istream, container.ContainerType())) {
			// If it is encountered on level 0 it is a syntax error
			if (m_container_level == 0)
				return Unexpected<ParseError>(m_current_line, "return Unexpected container end symbol");
			else
				m_container_level--;
			
			halt = true;
		}

		if (istream.fail()) {
			if (m_container_level > 0)
				return Unexpected<ParseError>(m_current_line, "return Unexpected EOF");
			else
				halt = true;
		}
	}
	return {};
}

StormByte::Expected<std::string, StormByte::Config::ParseError> Parser::ParseItemName(std::istream& istream) {
	const std::string name = GetStringIgnoringWS(istream);
	if (!Item::IsNameValid(name)) {
		return Unexpected<ParseError>(m_current_line, "Invalid item name: " + name);
	}
	return name;
}

StormByte::Expected<StormByte::Config::Item::Type, StormByte::Config::ParseError> Parser::ParseType(std::istream& istream) {
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
			return Unexpected<ParseError>(m_current_line, "return Unexpected " + std::string(1, line[0]) + " when parsing item type");
		}
	}
	istream.seekg(start_position);
	return *type;
}

StormByte::Expected<StormByte::Config::Item::ContainerType, StormByte::Config::ParseError> Parser::ParseContainerType(std::istream& istream) {
	/**** THIS FUNCTION ONLY DETECTS TYPE NOT CHECKS FOR VALIDITY *****/
	char c = '\0';
	ConsumeWS(istream);
	istream.get(c);
	try {
		return Item::TypeFromStartCharacter(c);
	}
	catch (const StormByte::Exception&) {
		return Unexpected<ParseError>(m_current_line, "Unknown start character " + std::string(1, c) + " for container");
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
	StormByte::Expected<void, StormByte::Config::ParseError> Parse(std::istream& stream, Item::Group& root, const StormByte::Config::OnExistingAction& action, const HookFunctions& before, const HookFunctions& after, const OptionalFailureHook& on_failure) {
		return Parser::Parse(stream, root, action, before, after, on_failure);
	}

	StormByte::Expected<void, StormByte::Config::ParseError> Parse(const std::string& string, Item::Group& root, const StormByte::Config::OnExistingAction& action, const HookFunctions& before, const HookFunctions& after, const OptionalFailureHook& on_failure) {
		return Parser::Parse(string, root, action, before, after, on_failure);
	}
}