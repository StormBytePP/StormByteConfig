#include <config/comment/base.hxx>

using namespace StormByte::Config::Comment;

Base::Base(const std::string& comment):m_comment(comment) {}

Base::Base(std::string&& comment):m_comment(std::move(comment)) {}