#include <config/container/group.hxx>

using namespace StormByte::Config::Container;

Group::PointerType Group::Clone() const {
	return MakePointer<Group>(*this);
}

Group::PointerType Group::Move() {
	return MakePointer<Group>(std::move(*this));
}

StormByte::Config::Item& Group::Add(Item&& item, const OnExistingAction& on_existing) {
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
