#include <config/list.hxx>

using namespace StormByte::Config;

std::shared_ptr<Container> List::Clone() const {
	return std::make_shared<List>(*this);
}

std::shared_ptr<Container> List::Move() {
	return std::make_shared<List>(std::move(*this));
}

Item& List::Add(Item&& item, const OnExistingAction& on_existing) {
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