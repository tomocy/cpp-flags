#include "src/flags/parser.h"

#include <cassert>
#include <map>
#include <string>
#include <vector>

#include "src/flags/analysis/analysis.h"
#include "src/flags/exceptions.h"
#include "src/flags/flag.h"

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