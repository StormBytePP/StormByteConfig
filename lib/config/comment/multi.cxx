#include <config/comment/multi.hxx>

#include <sstream>

using namespace StormByte::Config::Comment;

std::string MultiLine::Serialize(const int&) const noexcept {
	// The multiline comments already have the indent
	std::stringstream ss(*this);
	std::string item;
	std::string serial = "/*";
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

MultiLine::PointerType MultiLine::Clone() const {
	return MakePointer<MultiLine>(*this);
}

MultiLine::PointerType MultiLine::Move() {
	return MakePointer<MultiLine>(std::move(*this));
}