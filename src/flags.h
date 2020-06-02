#ifndef TOMOCY_CPP_FLAGS_FLAGS_H
#define TOMOCY_CPP_FLAGS_FLAGS_H

#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "src/analysis.h"
#include "src/exceptions.h"

namespace flags {
class Value {
 public:
  virtual ~Value() {}
  virtual Value& operator=(const std::string& value) = 0;
  virtual std::string Type() const noexcept = 0;
  virtual std::string ToString() const noexcept = 0;
};
}  // namespace flags

namespace flags {
class String : public Value {
 public:
  static std::unique_ptr<String> Make(const std::string& value) noexcept;

  explicit String(const std::string& value) noexcept;

  Value& operator=(const std::string& value) noexcept override;

  std::string Type() const noexcept override;

  std::string ToString() const noexcept override;

  const std::string& Value() const noexcept;

 private:
  std::string value;
};
}  // namespace flags

namespace flags {
class Int : public Value {
 public:
  static std::unique_ptr<Int> Make(int value) noexcept;

  explicit Int(int value) noexcept;

  Value& operator=(const std::string& value) override;

  std::string Type() const noexcept override;

  std::string ToString() const noexcept override;

  int Value() const noexcept;

 private:
  int value;
};
}  // namespace flags

namespace flags {
class Bool : public Value {
 public:
  static std::unique_ptr<Bool> Make(bool value) noexcept;

  explicit Bool(bool value) noexcept;

  Value& operator=(const std::string& value) override;

  std::string Type() const noexcept override;

  std::string ToString() const noexcept override;

  bool Value() const noexcept;

 private:
  bool value;
};
}  // namespace flags

namespace flags {
class Flag {
 public:
  using value_t = std::unique_ptr<Value>;

  Flag() = default;

  Flag(const std::string& name, value_t value);

  Flag(const std::string& name, value_t value, const std::string& description);

  std::string Usage() const noexcept;

  const std::string& Name() const noexcept;

  void SetValue(const std::string& value);

  template <typename T>
  std::tuple<T, bool> TryToGet() const;

  template <typename T>
  T Get() const;

 private:
  const std::string& ValidateName(const std::string& name) const;
  value_t ValidateValue(value_t&& value) const;
  const std::string& ValidateDescription(const std::string& description) const;

  std::string name;
  value_t value;
  std::string description;
};
}  // namespace flags

namespace flags {
bool IsBoolFlag(const Flag& flag) noexcept;
}  // namespace flags

namespace flags {
class FlagSet {
 public:
  FlagSet(const std::string& name);

  void AddFlag(Flag&& flag);

  const Flag& GetFlag(const std::string& name) const;

  const std::vector<std::string>& Args() const noexcept;

  void Parse(const std::vector<std::string>& args);

  void Parse(const std::string& args);

  std::string Usage() const noexcept;

 private:
  const std::string& ValidateName(const std::string& name) const;

  std::string name;
  std::map<std::string, Flag> flags;
  std::vector<std::string> args;
};
}  // namespace flags

namespace flags {
using namespace analysis;
class Parser {
 public:
  Parser(const std::vector<char>& src, std::map<std::string, Flag>& flags,
         std::vector<std::string>& args) noexcept;

  Parser(const Lexer& lexer, std::map<std::string, Flag>& flags,
         std::vector<std::string>& args) noexcept;

  void Parse();

 private:
  void ParseFlag();

  void ParseArgs() noexcept;

  void SkipWhitespaces() noexcept;

  bool DoHave(TokenKind kind) const noexcept;

  std::string ReadArg() noexcept;

  std::string ReadString();

  void ReadToken() noexcept;

  Lexer lexer;
  Token curr_token;
  Token next_token;

  std::map<std::string, Flag>& flags;
  std::vector<std::string>& args;
};
}  // namespace flags

#endif