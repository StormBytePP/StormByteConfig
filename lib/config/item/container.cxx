#include <config/item/container.hxx>
#include <util/string.hxx>

#include <regex>

using namespace StormByte::Config::Item;

Container::Container(const std::string& name):Base(name) {}

Container::Container(std::string&& name):Base(std::move(name)) {}

Base& Container::operator[](const size_t& index) {
	return const_cast<Base&>(static_cast<const Container&>(*this)[index]);
}

const Base& Container::operator[](const size_t& index) const {
	if (index >= m_items.size())
		throw OutOfBounds(index, m_items.size());
	return *m_items[index];
}

Base& Container::operator[](const std::string& path) {
	return const_cast<Base&>(static_cast<const Container&>(*this)[path]);
}

bool Container::operator==(const Container& container) const noexcept {
	// Perform the base class comparison
	if (Base::operator!=(container)) {
		return false;
	}

	// Perform type comparison
	if (ContainerType() != container.ContainerType()) {
		return false;
	}

	// Compare sizes of `m_items` first
	if (m_items.size() != container.m_items.size()) {
		return false;
	}

	// Compare each item in `m_items`
	for (size_t i = 0; i < m_items.size(); ++i) {
		if (m_items[i]->Type() != container.m_items[i]->Type() || *m_items[i] != *container.m_items[i]) {
			return false;
		}
	}

	// All comparisons are equal
	return true;
}

Base& Container::Add(Base::PointerType item, const OnExistingAction& on_existing) {
	Base::PointerType i = this->BeforeAdditionActions(item, on_existing);

	if (i)
		return *i;
	else {
		m_items.push_back(item);
		return *m_items.back();
	}
}

bool Container::Exists(const std::string& path) const {
	try {
		LookUp(path);
		return true;
	} catch(const Exception&) {
		return false;
	}
}

void Container::Remove(const size_t& index) {
	if (index >= m_items.size())
		throw OutOfBounds(index, m_items.size());
	m_items.erase(m_items.begin() + index);
}

void Container::Remove(const std::string& path)  {
	auto path_queue = Util::String::Explode(path, '/');
	Remove(path_queue);
}

std::string Container::Serialize(const int& indent_level) const noexcept {
	const auto enclosure_characters = EnclosureCharacters(ContainerType());
	std::string serial = Base::Serialize(indent_level) + std::string(1, enclosure_characters.first) + "\n";
	serial += ContentsToString(indent_level + 1);
	serial += Util::String::Indent(indent_level) + enclosure_characters.second;
	return serial;
}

size_t Container::Count() const noexcept {
	size_t count = 0;
	for (const auto& item : m_items) {
		switch(item->Type()) {
			case Type::Container:
				count += 1 + item->Value<Container>().Count();
				break;
			default:
				count++;
				break;
		}
	}
	return count;
}

std::string Container::ContentsToString(const int& indent_level) const noexcept {
	std::string serial = "";
	for (const auto& item : m_items)
		serial += item->Serialize(indent_level) + "\n";
	return serial;
}

bool Container::IsPathValid(const std::string& name) noexcept {
	static const std::regex name_regex(R"(^[A-Za-z][A-Za-z0-9_]*(/[A-Za-z0-9_]+)*$)");
	return std::regex_match(name, name_regex);
}

const Base& Container::LookUp(const std::string& path) const {
	auto path_queue = Util::String::Explode(path, '/');
	return LookUp(path_queue);
}

const Base& Container::LookUp(std::queue<std::string>& path) const {
	const std::string item_path = path.front();
	path.pop();
	if (path.size() == 0) {
		if (Util::String::IsNumeric(item_path)) {
			return *m_items.at(std::stoi(item_path));
		}
		else {
			const auto it = std::find_if(m_items.begin(), m_items.end(), [&item_path](const Base::PointerType& item) {
				const auto& name = item->Name();
				return name && item->Name().value() == item_path;
			});
			if (it != m_items.end())
				return **it;
			else
				throw ItemNotFound(item_path);
		}
	}
	else {
		// Recursive LookUp path
		const Base& item = Util::String::IsNumeric(item_path) ? operator[](std::stoi(item_path)) : operator[](item_path);
		if (item.Type() != Type::Container)
			throw Exception("Lookup path " + item_path + " applied to non container item");
		else {
			return static_cast<const Container&>(item).LookUp(path);
		}
	}
}

void Container::Remove(std::queue<std::string>& path) {
	std::string item_path = path.front();
	path.pop();
	if (path.size() == 0) {
		if (Util::String::IsNumeric(item_path))
			Remove(std::stoi(item_path));
		else {
			const auto it = std::find_if(m_items.begin(), m_items.end(), [&item_path](const Base::PointerType& item) {
				const auto& name = item->Name();
				return name && item->Name().value() == item_path;
			});
			if (it != m_items.end())
				m_items.erase(it);
			else
				throw ItemNotFound(item_path);
		}
	}
	else {
		// Recursive Remove path
		Base& item = Util::String::IsNumeric(item_path) ? operator[](std::stoi(item_path)) : operator[](item_path);
		if (item.Type() != Type::Container)
			throw Exception("Lookup path " + item_path + " applied to non container item");
		else {
			item.Value<Container>().Remove(path);
		}
	}
}