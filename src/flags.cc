#include "src/flags.h"

#include <memory>
#include <string>

namespace flags {
std::unique_ptr<String> String::Make(const std::string& value) noexcept {
  return std::make_unique<String>(value);
}

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
Flag::Flag(const std::string& name, value_t value)
    : Flag(name, std::move(value), "") {}

Flag::Flag(const std::string& name, value_t value,
           const std::string& description)
    : name(ValidateName(name)),
      value(ValidateValue(std::move(value))),
      description(ValidateDescription(description)) {}

std::string Flag::Usage() const noexcept {
  auto usage = "--" + name + " " + value->Type() + "  ";

  if (!description.empty()) {
    usage += description + " ";
  }

  usage += "(default: " + value->ToString() + ")";

  return usage;
}

const std::string& Flag::ValidateName(const std::string& name) const {
  if (name.empty()) {
    throw Exception("name should not be empty");
  }

  return name;
}

typename Flag::value_t Flag::ValidateValue(value_t&& value) const {
  if (!value) {
    throw Exception("value should not be null");
  }

  return std::move(value);
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