#include "src/analysis.h"

#include <string>
#include <vector>

namespace flags::analysis {
Token::Token(TokenKind kind, const std::string& literal) noexcept
    : kind(kind), literal(literal) {}

TokenKind Token::Kind() const noexcept { return kind; }

const std::string& Token::Literal() const noexcept { return literal; }
}  // namespace flags::analysis

namespace flags::analysis {
Lexer::Lexer(const std::vector<char>& src) noexcept : src(src), index(0) {}

Token Lexer::ReadToken() noexcept {
  auto curr = CurrentChar();

  if (curr == kCharEOF) {
    return ComposeSingleTokenAs(TokenKind::END_OF_FILE);
  }

  return ComposeSingleTokenAs(TokenKind::UNKNOWN);
}

Token Lexer::ComposeSingleTokenAs(TokenKind kind) {
  if (kind == TokenKind::END_OF_FILE) {
    return kTokenEOF;
  }

  return ComposeTokenAs(1, kind);
}

Token Lexer::ComposeTokenAs(int n, TokenKind kind) {
  auto literal = std::string{""};
  for (auto i = 0; i < n; ++i) {
    literal += std::string{CurrentChar()};
    ReadChar();
  }

  return Token(kind, literal);
}

void Lexer::ReadChar() noexcept {
  if (index >= src.size()) {
    return;
  }

  ++index;
}

char Lexer::CurrentChar() const noexcept {
  if (index >= src.size()) {
    return kCharEOF;
  }

  return src.at(index);
}
}  // namespace flags::analysis