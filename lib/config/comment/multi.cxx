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

std::shared_ptr<Comment> MultiLine::Clone() const {
	return std::make_shared<MultiLine>(*this);
}

std::shared_ptr<Comment> MultiLine::Move() {
	return std::make_shared<MultiLine>(std::move(*this));
}