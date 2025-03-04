#include <config/comment.hxx>
#include <config/group.hxx>
#include <config/list.hxx>
#include <config/exception.hxx>
#include <util/string.hxx>

#include <regex>

using namespace StormByte::Config;

Item::Item(const std::string& name, const Container& value):m_name(name), m_type(Type::Container), m_value(value.Clone()) {}

Item::Item(const Container& value):m_type(Type::Container), m_value(value.Clone()) {}

Item::Item(std::string&& name, Container&& value):m_name(std::move(name)), m_type(Type::Container), m_value(value.Move()) {}

Item::Item(Container&& value):m_type(Type::Container), m_value(value.Move()) {}

Item::Item(const Comment::Comment& value):m_type(Type::Comment), m_value(value.Clone()) {}

Item::Item(Comment::Comment&& value):m_type(Type::Comment), m_value(value.Move()) {}

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
			serial += NameEqualSignString() + std::to_string((int)m_value);
			break;
		case Type::String:
			#ifdef MSVC
			/** MSVC seems not to pick operator std::string here so a cast would not be valid **/
			serial += NameEqualSignString() + "\"" + m_value.Get<std::string>() + "\"";
			#else
			serial += NameEqualSignString() + "\"" + std::string((std::string)m_value) + "\"";
			#endif
			break;
		case Type::Double:
			serial += NameEqualSignString() + std::to_string((double)m_value);
			break;
		case Type::Bool:
			serial += NameEqualSignString() + std::string(m_value ? "true" : "false");
			break;
		case Type::Comment:
			serial += m_value.Get<CommentPTR>()->Serialize(indent_level);
			break;
		case Type::Container:
			serial += NameEqualSignString() + m_value.Get<ContainerPTR>()->Serialize(indent_level);
			break;
	}
	return serial + "\n";
}

size_t Item::Count() const noexcept {
	switch(m_type) {
		case Type::Container:
			return 1 + m_value.Get<ContainerPTR>()->Count();
		case Type::Comment:
			return m_value.Get<CommentPTR>()->Count();
		default:
			return 1;
	}
}

template<> const Container& Item::Value<Container>() const {
	return *m_value.Get<ContainerPTR>();
}

template<> Container& Item::Value<Container>() {
	return const_cast<Container&>(static_cast<const Item&>(*this).Value<Container>());
}
