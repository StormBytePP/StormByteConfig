#include <StormByte/config/item/group.hxx>
#include <StormByte/config/item/list.hxx>
#include <StormByte/config/item/value.hxx>
#include <StormByte/config/item/comment.hxx>
#include <StormByte/util/string.hxx>

#include <regex>

using namespace StormByte::Config::Item;

Base::Base(const std::string& name):m_name(name) {}

std::string Base::Serialize(const int& indent_level) const noexcept {
	std::string serialized = Util::String::Indent(indent_level);
	if (m_name)
		serialized += *m_name + " = ";
	return serialized;
}

namespace StormByte::Config::Item {
	bool IsNameValid(const std::string& name) noexcept {
		static const std::regex name_regex(R"(^[A-Za-z][A-Za-z0-9_]*$)");
		return std::regex_match(name, name_regex);
	}
}