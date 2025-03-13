#include <StormByte/config/item/group.hxx>

#include <algorithm>

using namespace StormByte::Config::Item;

Group::Group(const std::string& name):Container(name) {}

Group::Group(std::string&& name):Container(std::move(name)) {}

Base::PointerType Group::BeforeAdditionActions(Base::PointerType item, const OnExistingAction onexisting) {
	if (!item) {
		throw Exception("Tried to add a null pointer to Group!");
	}

	if (item->Type() != Type::Comment) {
		if (!item->Name().has_value())
			throw InvalidName();
		else if (!item->IsNameValid()) {
			throw InvalidName(item->Name().value(), ContainerType());
		}

		// Don't use Exists here for better speed
		const auto it = std::find_if(m_items.begin(), m_items.end(), [&item](const Base::PointerType& i) {
			return i->Type() != Type::Comment && i->Name().value() == item->Name().value();
		});
		if (it != m_items.end()) {
			switch (onexisting) {
				case OnExistingAction::Keep:
					return *it;
					break;
				case OnExistingAction::Overwrite:
					m_items.erase(it);
					break;
				case OnExistingAction::ThrowException:
					throw ItemNameAlreadyExists(*item->Name());
					break;
			}
		}
	}

	m_items.push_back(item);
	return m_items.back();
}
