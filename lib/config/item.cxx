#include <config/comment/base.hxx>
#include <config/container/group.hxx>
#include <config/container/list.hxx>
#include <config/exception.hxx>
#include <util/string.hxx>

#include <regex>

using namespace StormByte::Config;

Item::Item(const std::string& name, const Container::Base& value):m_name(name), m_type(Type::Container), m_value(value.Clone()) {}

Item::Item(const Container::Base& value):m_type(Type::Container), m_value(value.Clone()) {}

Item::Item(std::string&& name, Container::Base&& value):m_name(std::move(name)), m_type(Type::Container), m_value(value.Move()) {}

Item::Item(Container::Base&& value):m_type(Type::Container), m_value(value.Move()) {}

Item::Item(const Comment::Base& value):m_type(Type::Comment), m_value(value.Clone()) {}

Item::Item(Comment::Base&& value):m_type(Type::Comment), m_value(value.Move()) {}

Item::Item(const std::string& name, const std::string& value):m_name(name), m_type(Type::String), m_value(value) {}

Item::Item(const std::string& value):m_type(Type::String), m_value(value) {}

Item::Item(std::string&& name, std::string&& value):m_name(std::move(name)), m_type(Type::String), m_value(std::move(value)) {}

Item::Item(std::string&& value):m_type(Type::String), m_value(std::move(value)) {}

Item::Item(const char* name, const char* value):m_name(name), m_type(Type::String), m_value(std::string(value)) {}

Item::Item(const char* value):m_type(Type::String), m_value(std::string(value)) {}

Item::Item(const std::string& name, const int& value):m_name(name), m_type(Type::Integer), m_value(value) {}

Item::Item(const int& value):m_type(Type::Integer), m_value(value) {}

Item::Item(const std::string& name, const double& value):m_name(name), m_type(Type::Double), m_value(value) {}

Item::Item(const double& value):m_type(Type::Double), m_value(value) {}

Item::Item(const std::string& name, bool value):m_name(name), m_type(Type::Bool), m_value(value) {}

Item::Item(bool value):m_type(Type::Bool), m_value(value) {}

bool Item::IsNameValid(const std::string& name) noexcept {
	std::regex name_regex(R"(^[A-Za-z][A-Za-z0-9_]*$)");
	return std::regex_match(name, name_regex);
}

bool Item::IsPathValid(const std::string& name) noexcept {
    std::regex name_regex(R"(^[A-Za-z][A-Za-z0-9_]*(/[A-Za-z0-9_]+)*$)");
	return std::regex_match(name, name_regex);
}

std::string Item::ContentsToString(const int& indent_level) const noexcept {
	std::string serial = Util::String::Indent(indent_level);
	switch (m_type) {
		case Type::Integer:
			serial += NameEqualSignString() + std::to_string(m_value.Get<int>());
			break;
		case Type::String:
			serial += NameEqualSignString() + "\"" + m_value.Get<std::string>() + "\"";
			break;
		case Type::Double:
			serial += NameEqualSignString() + std::to_string(m_value.Get<double>());
			break;
		case Type::Bool:
			serial += NameEqualSignString() + std::string(m_value.Get<bool>() ? "true" : "false");
			break;
		case Type::Comment:
			serial += m_value.Get<Comment::Base>().Serialize(indent_level);
			break;
		case Type::Container:
			serial += NameEqualSignString() + m_value.Get<Container::Base>().Serialize(indent_level);
			break;
	}
	return serial + "\n";
}
