#ifndef TOMOCY_CPP_FLAGS_FLAGS_FLAGS_H
#define TOMOCY_CPP_FLAGS_FLAGS_FLAGS_H

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "src/flags/analysis/analysis.h"
#include "src/flags/exceptions.h"
#include "src/flags/flag.h"
#include "src/flags/values.h"

namespace flags {
bool IsBoolFlag(const Flag& flag) noexcept;
}  // namespace flags

namespace flags {
std::string GenerateDefaultFlagSetUsage(
    const std::string& name, const std::map<std::string, Flag>& flags);
}

namespace flags {
class FlagSet {
 public:
  using usage_generator_t = std::function<std::string(
      const std::string& name, const std::map<std::string, Flag>& flags)>;

  FlagSet(const std::string& name);

  void AddFlag(Flag&& flag);

  const Flag& GetFlag(const std::string& name) const;

  const std::vector<std::string>& Args() const noexcept;

  void Parse(const std::vector<std::string>& args);

  void Parse(const std::string& args);

  std::string Usage() const noexcept;

  std::string Usage(const usage_generator_t& generator) const noexcept;

  size_t FlagSize() const noexcept;

  size_t ArgSize() const noexcept;

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