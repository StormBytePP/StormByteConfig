#include <config/exception.hxx>

using namespace StormByte::Config;

Exception::Exception(const std::string& msg):StormByte::Exception(msg) {}

WrongValueTypeConversion::WrongValueTypeConversion(const std::string& srctype, const std::string& method):
Exception(method + " conversion failed for " + srctype) {}

ValueFailure::ValueFailure(const std::string& srctype, const std::string& dsttype):
Exception("Try to add/set " + dsttype + " value which is of type " + srctype) {}

InvalidName::InvalidName(const std::string& name):
Exception("Invalid name " + name + " given as identifier, only alfanumeric is allowed") {}

InvalidPath::InvalidPath(const std::string& path):
Exception("Invalid path " + path + " given") {}

ParseError::ParseError(const unsigned int& line, const std::string& reason):
Exception("Parse error on line " + std::to_string(line) + ": " + reason) {}

ItemNotFound::ItemNotFound(const std::string& name):
Exception("Item " + name + " do not exist") {}

ItemAlreadyExists::ItemAlreadyExists():
Exception("Another item with the same contents already exists") {}

ItemNameAlreadyExists::ItemNameAlreadyExists(const std::string& name):
Exception("Another item with name " + name + " already exists") {}

OutOfBounds::OutOfBounds(const size_t& index, const size_t& size):
Exception("Index " + std::to_string(index) + " is out of bounds when size is " + std::to_string(size)) {}