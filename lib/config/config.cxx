#include <config/config.hxx>
#include <config/parser/parser.hxx>

using namespace StormByte::Config;

Config::Config():m_on_existing_action(OnExistingAction::ThrowException) {}

Config& Config::operator<<(const Config& source) {
	// We will not use serialize for performance reasons
	for (const auto& item: source.Items())
		Add(*item->Clone());
		
	return *this;
}

void Config::operator<<(std::istream& istream) { // 1
	auto res = Parser::Parse(istream, m_root, m_on_existing_action, m_before_read_hooks, m_after_read_hooks, m_on_parse_failure_hook);
	if (!res)
		throw *res.error();
}

void Config::operator<<(const std::string& str) { // 2
	auto res = Parser::Parse(str, m_root, m_on_existing_action, m_before_read_hooks, m_after_read_hooks, m_on_parse_failure_hook);
	if (!res)
		throw *res.error();
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
	for (const auto& item : Items()) {
		serialized += item->Serialize(0) + "\n";
	}
	return serialized;
}
