#include <config/comment/single.hxx>

using namespace StormByte::Config::Comment;

SingleLine::SingleLine(const std::string& comment):Base(comment) {}

SingleLine::SingleLine(std::string&& comment):Base(std::move(comment)) {}

std::string SingleLine::Serialize(const int&) const noexcept {
	return "#" + m_comment; // It is expected to start alreadyu indented
}

SingleLine::PointerType SingleLine::Clone() const {
	return MakePointer<SingleLine>(*this);
}

SingleLine::PointerType SingleLine::Move() {
	return MakePointer<SingleLine>(std::move(*this));
}