#include <config/item/comment.hxx>
#include <util/string.hxx>

#include <sstream>

namespace StormByte::Config::Item {
	template<>
	std::string STORMBYTE_CONFIG_PUBLIC Comment<CommentType::SingleLineBash>::Serialize(const int& indent_level) const noexcept {
		return Util::String::Indent(indent_level) + "#" + m_value; // It is expected to start already indented
	}
	template class Comment<CommentType::SingleLineBash>;

	template<>
	std::string STORMBYTE_CONFIG_PUBLIC Comment<CommentType::SingleLineC>::Serialize(const int& indent_level) const noexcept {
		return Util::String::Indent(indent_level) + "//" + m_value; // It is expected to start already indented
	}
	template class Comment<CommentType::SingleLineC>;

	template<>
	std::string STORMBYTE_CONFIG_PUBLIC Comment<CommentType::MultiLineC>::Serialize(const int& indent_level) const noexcept {
		// The MultiLineC comments already have the indent
		std::stringstream ss(m_value);
		std::string item;
		std::string serial = Util::String::Indent(indent_level) + "/*";
		std::getline(ss, item);
		serial += item;
		if (!ss.eof()) {
			serial += "\n";
			while (std::getline(ss, item)) {
				serial += item; 
				if (!ss.eof()) serial += "\n";
			}
		}
		return serial + "*/";
	}
	template class Comment<CommentType::MultiLineC>;
}