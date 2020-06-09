#ifndef TOMOCY_CPP_FLAGS_FLAGS_PARSER_H
#define TOMOCY_CPP_FLAGS_FLAGS_PARSER_H

#include <map>
#include <string>
#include <vector>

#include "src/flags/analysis/analysis.h"
#include "src/flags/flag.h"

namespace flags {
bool IsBoolFlag(const Flag& flag) noexcept;
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