#ifndef TOMOCY_CPP_FLAGS_ANALYSIS_H
#define TOMOCY_CPP_FLAGS_ANALYSIS_H

#include <string>
#include <vector>

namespace flags::analysis {
enum class TokenKind {
  END_OF_FILE,
  UNKNOWN,
  WHITESPACE,
  SHORT_FLAG,
  LONG_FLAG,
  EQUAL,
  STRING
};
}

namespace flags::analysis {
class Token {
 public:
  Token(TokenKind kind, const std::string& literal) noexcept;

  TokenKind Kind() const noexcept;

  const std::string& Literal() const noexcept;

 private:
  TokenKind kind;
  std::string literal;
};
}  // namespace flags::analysis

namespace flags::analysis {
const Token kTokenEOF = Token(TokenKind::END_OF_FILE, "\x00");
const Token kTokenShortFlag = Token(TokenKind::SHORT_FLAG, "-");
const Token kTokenLongFlag = Token(TokenKind::LONG_FLAG, "--");
const Token kTokenEqual = Token(TokenKind::EQUAL, "=");

const char kCharEOF = 0;
}  // namespace flags::analysis

namespace flags::analysis {
class Lexer {
 public:
  explicit Lexer(const std::vector<char>& src) noexcept;

  Token ReadToken() noexcept;

 private:
  Token ComposeSingleTokenAs(TokenKind kind) noexcept;

  Token ComposeDualTokenAs(TokenKind kind) noexcept;

  Token ComposeTokenAs(int n, TokenKind kind) noexcept;

  Token ComposeString() noexcept;

  std::string ReadLetters() noexcept;

  bool DoHaveWhitespace() const noexcept;

  bool DoHaveShortFlag() const noexcept;

  bool DoHaveLongFlag() const noexcept;

  bool DoHaveLetter() const noexcept;

  bool DoHave(char c) const noexcept;

  bool WillHave(char c) const noexcept;

  char CurrentChar() const noexcept;

  char NextChar() const noexcept;

  void ReadChar() noexcept;

  std::vector<char> src;
  size_t index;
};
}  // namespace flags::analysis

#endif