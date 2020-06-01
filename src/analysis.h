#ifndef TOMOCY_CPP_FLAGS_ANALYSIS_H
#define TOMOCY_CPP_FLAGS_ANALYSIS_H

#include <string>
#include <vector>

namespace flags::analysis {
enum class TokenKind { END_OF_FILE, UNKNOWN };
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

const char kCharEOF = 0;
}  // namespace flags::analysis

namespace flags::analysis {
class Lexer {
 public:
  explicit Lexer(const std::vector<char>& src) noexcept;

  Token ReadToken() noexcept;

 private:
  Token ComposeSingleTokenAs(TokenKind kind);

  Token ComposeTokenAs(int n, TokenKind kind);

  char CurrentChar() const noexcept;

  void ReadChar() noexcept;

  std::vector<char> src;
  size_t index;
};
}  // namespace flags::analysis

#endif