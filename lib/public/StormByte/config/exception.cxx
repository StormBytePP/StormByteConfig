#include <StormByte/config/exception.hxx>

using namespace StormByte::Config;

Exception::Exception(const std::string& msg):StormByte::Exception(msg) {}

WrongValueTypeConversion::WrongValueTypeConversion(const std::string& srctype, const std::string& dsttype):
Exception("Conversion failed from " +srctype + " to " + dsttype) {}

ValueFailure::ValueFailure(const std::string& srctype, const std::string& dsttype):
Exception("Try to add/set " + dsttype + " value which is of type " + srctype) {}

InvalidName::InvalidName():
Exception("Empty names are not allowed in Group") {}

InvalidName::InvalidName(const std::string& name, const Item::ContainerType& containertype):
Exception(containertype == Item::ContainerType::Group ? "Invalid name " + name + " given as identifier, only alfanumeric is allowed" : "Lists do not accept named items (given " + name + ")") {}

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