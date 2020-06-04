#include "src/flags/values.h"

#include <memory>
#include <string>

#include "src/flags/exceptions.h"

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
std::unique_ptr<Int> Int::Make(int value) noexcept {
  return std::make_unique<Int>(value);
}

Int::Int(int value) noexcept : value(value) {}

Value& Int::operator=(const std::string& value) {
  try {
    this->value = std::stoi(value);
    return *this;
  } catch (const std::invalid_argument& e) {
    throw Exception("flag value \"" + value +
                    "\" should be castable to int: " + e.what());
  } catch (const std::out_of_range& e) {
    throw Exception("flag value \"" + value +
                    "\" should be castable to int: " + e.what());
  }
}

std::string Int::Type() const noexcept { return "int"; }

std::string Int::ToString() const noexcept { return std::to_string(value); }

int Int::Value() const noexcept { return value; }
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