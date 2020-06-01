#include "src/flags.h"

#include <memory>
#include <string>

namespace flags {
String::String(const std::string& value) noexcept : value(value) {}

Value& String::operator=(const std::string& value) noexcept {
  this->value = value;
  return *this;
}

std::string String::Type() const noexcept { return "string"; }

std::string String::ToString() const noexcept {
  return std::string("\"") + value + std::string("\"");
}
}  // namespace flags

namespace flags {
Flag::Flag(const std::string& name, std::unique_ptr<Value> value,
           const std::string& description)
    : name(ValidateName(name)),
      value(std::move(value)),
      description(ValidateDescription(description)) {}

std::string Flag::Usage() const noexcept {
  return "--" + name + " " + value->Type() + "  " + description +
         " (default: " + value->ToString() + ")";
}

const std::string& Flag::ValidateName(const std::string& name) const {
  if (name.empty()) {
    throw Exception("name should not be empty");
  }

  return name;
}

const std::string& Flag::ValidateDescription(
    const std::string& description) const {
  return description;
}
}  // namespace flags

namespace flags {
Exception::Exception(const std::string& msg) noexcept : msg(msg) {}

const std::string& Exception::What() const noexcept { return msg; }
}  // namespace flags