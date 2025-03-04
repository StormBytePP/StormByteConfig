#include <config/group.hxx>

using namespace StormByte::Config;

std::shared_ptr<Container> Group::Clone() const {
	return std::make_shared<Group>(*this);
}

std::shared_ptr<Container> Group::Move() {
	return std::make_shared<Group>(std::move(*this));
}

Item& Group::Add(Item&& item, const OnExistingAction& on_existing) {
	if (item.GetType() != Item::Type::Comment) {
		const auto& item_name = item.Name();
		if (!item_name) {
			throw InvalidName("Group does not allow unnamed items");
		}
		if (!Item::IsNameValid(*item_name))
			throw InvalidName(*item_name);
	}

	if (item.GetType() != Item::Type::Comment && Exists(*item.Name())) {
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

	m_items.push_back(std::move(item));
	return m_items.back();
}
