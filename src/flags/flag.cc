#include "src/flags/flag.h"

#include "src/flags/exceptions.h"
#include "src/flags/values.h"

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

void Flag::SetValue(const std::string& value) { (*this->value) = value; }

template <>
std::tuple<std::string, bool> Flag::TryToGet<std::string>() const noexcept {
  auto casted = dynamic_cast<String*>(value.get());
  if (casted == nullptr) {
    return {"", false};
  }

  return {casted->Value(), true};
}

template <>
std::string Flag::Get<std::string>() const {
  auto [value, ok] = TryToGet<std::string>();
  if (!ok) {
    throw Exception("flag does not have a value which is castable to string");
  }

  return value;
}

template <>
std::tuple<int, bool> Flag::TryToGet<int>() const noexcept {
  auto casted = dynamic_cast<Int*>(value.get());
  if (casted == nullptr) {
    return {0, false};
  }

  return {casted->Value(), true};
}

template <>
int Flag::Get<int>() const {
  auto [value, ok] = TryToGet<int>();
  if (!ok) {
    throw Exception("flag does not have a value which is castable to int");
  }

  return value;
}

template <>
std::tuple<bool, bool> Flag::TryToGet<bool>() const noexcept {
  auto casted = dynamic_cast<Bool*>(value.get());
  if (casted == nullptr) {
    return {false, false};
  }

  return {casted->Value(), true};
}

template <>
bool Flag::Get<bool>() const {
  auto [value, ok] = TryToGet<bool>();
  if (!ok) {
    throw Exception("flag does not have a value which is castable to bool");
  }

  return value;
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