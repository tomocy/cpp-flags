#include "src/flags.h"

#include <cassert>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "src/analysis.h"

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

void Flag::SetValue(const std::string& value) { (*this->value) = value; }

template <>
std::tuple<std::string, bool> Flag::TryToGet<std::string>() const {
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
std::tuple<bool, bool> Flag::TryToGet<bool>() const {
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

namespace flags {
bool IsBoolFlag(const Flag& flag) noexcept {
  try {
    flag.Get<bool>();
    return true;
  } catch (const Exception&) {
    return false;
  }
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

const Flag& FlagSet::GetFlag(const std::string& name) const {
  if (flags.find(name) == flags.end()) {
    throw Exception("flag \"" + name + "\" is not added");
  }

  return flags.at(name);
}

const std::vector<std::string>& FlagSet::Args() const noexcept { return args; }

void FlagSet::Parse(const std::vector<std::string>& args) {
  std::ostringstream joined;
  std::copy(std::begin(args), std::end(args),
            std::ostream_iterator<std::string>(joined, " "));

  auto src = joined.str();
  src = src.erase(src.size() - 1, 1);

  Parse(src);
}

void FlagSet::Parse(const std::string& args) {
  auto parser = Parser(std::vector<char>(std::begin(args), std::end(args)),
                       flags, this->args);

  parser.Parse();
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
Parser::Parser(const std::vector<char>& src, std::map<std::string, Flag>& flags,
               std::vector<std::string>& args) noexcept
    : Parser(Lexer(src), flags, args) {}

Parser::Parser(const Lexer& lexer, std::map<std::string, Flag>& flags,
               std::vector<std::string>& args) noexcept
    : lexer(lexer),
      curr_token(kTokenEOF),
      next_token(kTokenEOF),
      flags(flags),
      args(args) {
  ReadToken();
  ReadToken();
}

void Parser::Parse() {
  while (true) {
    SkipWhitespaces();

    switch (curr_token.Kind()) {
      case TokenKind::END_OF_FILE:
        return;
      case TokenKind::SHORT_FLAG:
      case TokenKind::LONG_FLAG:
        ParseFlag();
        break;
      case TokenKind::STRING:
        ParseArgs();
        return;
      default:
        throw Exception("token \"" + curr_token.Literal() + "\" is unknown");
    }
  }
}

void Parser::ParseFlag() {
  assert(DoHave(TokenKind::SHORT_FLAG) || DoHave(TokenKind::LONG_FLAG));

  ReadToken();

  if (!DoHave(TokenKind::STRING)) {
    throw Exception("the name of flag should be provided");
  }
  auto name = ReadString();

  if (flags.find(name) == flags.end()) {
    throw Exception("unknown flag \"" + name + "\" is provided");
  }
  auto& flag = flags.at(name);

  if (IsBoolFlag(flag)) {
    flag.SetValue("true");
    return;
  }

  if (!DoHave(TokenKind::WHITESPACE) && !DoHave(TokenKind::EQUAL)) {
    throw Exception("the value of flag \"" + name + "\" should be provided");
  }
  ReadToken();

  auto value = ReadString();

  flag.SetValue(value);
}

void Parser::ParseArgs() noexcept {
  while (!DoHave(TokenKind::END_OF_FILE)) {
    SkipWhitespaces();

    auto arg = ReadArg();
    std::cout << arg << std::endl;
    args.push_back(arg);
  }
}

void Parser::SkipWhitespaces() noexcept {
  while (DoHave(TokenKind::WHITESPACE)) {
    ReadToken();
  }
}

bool Parser::DoHave(TokenKind kind) const noexcept {
  return curr_token.Kind() == kind;
}

std::string Parser::ReadArg() noexcept {
  auto arg = std::string("");
  while (!DoHave(TokenKind::WHITESPACE) && !DoHave(TokenKind::END_OF_FILE)) {
    arg += curr_token.Literal();
    ReadToken();
  }

  return arg;
}

std::string Parser::ReadString() {
  assert(DoHave(TokenKind::STRING));

  auto literal = curr_token.Literal();

  ReadToken();

  return literal;
}

void Parser::ReadToken() noexcept {
  curr_token = next_token;
  next_token = lexer.ReadToken();
}
}  // namespace flags

namespace flags {
Exception::Exception(const std::string& msg) noexcept : msg(msg) {}

const std::string& Exception::What() const noexcept { return msg; }
}  // namespace flags