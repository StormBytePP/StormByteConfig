#include <config/container/list.hxx>

using namespace StormByte::Config::Container;

List::PointerType List::Clone() const {
	return MakePointer<List>(*this);
}

List::PointerType List::Move() {
	return MakePointer<List>(std::move(*this));
}

StormByte::Config::Item& List::Add(Item&& item, const OnExistingAction& on_existing) {
	if (item.GetType() != Item::Type::Comment && item.Name()) {
		throw InvalidName("List does not allow named items");
	}	

	if (item.GetType() != Item::Type::Comment) {
		if (std::find(m_items.begin(), m_items.end(), item) != m_items.end()) {
			switch (on_existing) {
				case OnExistingAction::Keep:
					return Child(*item.Name());
					break;
				case OnExistingAction::Overwrite: {
					Remove(*item.Name());
					break;
				}
				case OnExistingAction::ThrowException:
					throw Config::ItemAlreadyExists();
					break;
			}
		}
	}

	m_items.push_back(std::move(item));
	return m_items.back();
}