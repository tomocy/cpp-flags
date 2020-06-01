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

const std::string& String::Value() const noexcept { return value; }
}  // namespace flags

namespace flags {
std::unique_ptr<Bool> Bool::Make(bool value) noexcept {
  return std::make_unique<Bool>(value);
}

Bool::Bool(bool value) noexcept : value(value) {}

Value& Bool::operator=(const std::string& value) {
  if (value == "true") {
    this->value = true;
    return *this;
  }
  if (value == "false") {
    this->value = false;
    return *this;
  }

  throw Exception("flag value \"" + value + "\" should be castable to bool");
}

std::string Bool::Type() const noexcept { return "bool"; }

std::string Bool::ToString() const noexcept {
  return (value) ? "true" : "false";
}

bool Bool::Value() const noexcept { return value; }
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

const std::string& Flag::Name() const noexcept { return name; }

template <>
std::string Flag::Get<std::string>() const {
  auto casted = dynamic_cast<String*>(value.get());
  if (casted == nullptr) {
    throw Exception("flag does not have a value which is castable to string");
  }

  return casted->Value();
}

template <>
bool Flag::Get<bool>() const {
  auto casted = dynamic_cast<Bool*>(value.get());
  if (casted == nullptr) {
    throw Exception("flag does not have a value which is castable to bool");
  }

  return casted->Value();
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
FlagSet::FlagSet(const std::string& name) : name(ValidateName(name)) {}

void FlagSet::AddFlag(Flag&& flag) {
  if (flags.find(flag.Name()) != flags.end()) {
    throw Exception("flag \"" + flag.Name() + "\" is already added");
  }

  flags[flag.Name()] = std::move(flag);
}

std::string FlagSet::Usage() const noexcept {
  auto usage = name + "\n";

  for (auto iter = flags.begin(); iter != flags.end(); ++iter) {
    usage += "  " + iter->second.Usage() + "\n";
  }

  return usage.erase(usage.size() - 1, 1);
}

const std::string& FlagSet::ValidateName(const std::string& name) const {
  if (name.empty()) {
    throw Exception("name should not be empty");
  }

  return name;
}
}  // namespace flags

namespace flags {
Exception::Exception(const std::string& msg) noexcept : msg(msg) {}

const std::string& Exception::What() const noexcept { return msg; }
}  // namespace flags