#include <config/container/base.hxx>
#include <config/item.hxx>

#include <regex>

using namespace StormByte::Config::Container;

const StormByte::Config::Item& Base::Child(const std::string& name) const {
	auto it = std::find_if(
		m_items.begin(),
		m_items.end(),
		[&name](const Item& i) {
			bool equal;
			try {
				equal = i.GetType() != Item::Type::Comment && i.Name() == name;
			}
			catch(const Exception&) {
				equal = false;
			}
			return equal;
		}
	);
	if (it != m_items.end()) {
		return *it;
	}
	else
		throw ItemNotFound(name);
}

bool Base::Exists(const std::string& path) const {
	try {
		LookUp(path);
		return true;
	} catch(const Exception&) {
		return false;
	}
}

std::string Base::Serialize(const int& indent_level) const noexcept {
	const auto enclosure_characters = EnclosureCharacters(m_type);
	std::string serial = std::string(1, enclosure_characters.first) + "\n";
	serial += ContentsToString(indent_level + 1);
	serial += Util::String::Indent(indent_level) + enclosure_characters.second;
	return serial;
}

size_t Base::Count() const noexcept {
	size_t count = 0;
	for (const auto& item : m_items) {
		count += item.Count();
	}
	return count;
}

std::string Base::ContentsToString(const int& indent_level) const noexcept {
	std::string serial = "";
	for (auto it = m_items.begin(); it != m_items.end(); it++)
		serial += it->Serialize(indent_level);
	return serial;
}

bool Base::IsPathValid(const std::string& name) noexcept {
	std::regex name_regex(R"(^[A-Za-z][A-Za-z0-9_]*(/[A-Za-z0-9_]+)*$)");
	return std::regex_match(name, name_regex);
}

const StormByte::Config::Item& Base::LookUp(std::queue<std::string>& path) const {
	std::string item_path = path.front();
	path.pop();
	if (path.size() == 0) {
		if (Util::String::IsNumeric(item_path)) {
			return operator[](std::stoi(item_path));
		}
		else {
			return Child(item_path);
		}
	}
	else {
		// Recursive LookUp path
		if (Util::String::IsNumeric(item_path))
			return operator[](std::stoi(item_path)).Value().Get<Container::Base>().LookUp(path);
		else
			return Child(item_path).Value().Get<Container::Base>().LookUp(path);
	}
}

void Base::Remove(std::queue<std::string>& path) {
	std::string item_path = path.front();
	path.pop();
	if (path.size() == 0) {
		if (Util::String::IsNumeric(item_path))
			Remove(std::stoi(item_path));
		else {
			// Remove item
			m_items.erase(std::remove_if(m_items.begin(), m_items.end(), [&item_path](const Item& item) {
				return item.Name() == item_path;
			}), m_items.end());
		}
	}
	else {
		// Recursive Remove path
		Child(item_path).Value().Get<Container::Base>().Remove(path);
	}
}