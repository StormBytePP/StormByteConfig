#include <config/comment/single.hxx>

using namespace StormByte::Config::Comment;

std::string SingleLine::Serialize(const int&) const noexcept {
	return "#" + m_comment; // It is expected to start alreadyu indented
}

std::shared_ptr<Comment> SingleLine::Clone() const {
	return std::make_shared<SingleLine>(*this);
}

std::shared_ptr<Comment> SingleLine::Move() {
	return std::make_shared<SingleLine>(std::move(*this));
}