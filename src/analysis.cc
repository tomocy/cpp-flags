#include "src/analysis.h"

#include <iostream>
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
  SkipWhitespaces();

  if (DoHave(kCharEOF)) {
    return ComposeSingleTokenAs(TokenKind::END_OF_FILE);
  }

  if (DoHaveShortFlag()) {
    return ComposeSingleTokenAs(TokenKind::SHORT_FLAG);
  }

  if (DoHaveLongFlag()) {
    return ComposeDualTokenAs(TokenKind::LONG_FLAG);
  }

  if (DoHaveLetter()) {
    return ComposeString();
  }

  return ComposeSingleTokenAs(TokenKind::UNKNOWN);
}

Token Lexer::ComposeSingleTokenAs(TokenKind kind) noexcept {
  if (kind == TokenKind::END_OF_FILE) {
    ReadChar();
    return kTokenEOF;
  }

  if (kind == TokenKind::SHORT_FLAG) {
    ReadChar();
    return kTokenShortFlag;
  }

  return ComposeTokenAs(1, kind);
}

Token Lexer::ComposeDualTokenAs(TokenKind kind) noexcept {
  if (kind == TokenKind::LONG_FLAG) {
    ReadChar();
    ReadChar();
    return kTokenLongFlag;
  }

  return ComposeTokenAs(2, kind);
}

Token Lexer::ComposeTokenAs(int n, TokenKind kind) noexcept {
  auto literal = std::string{""};
  for (auto i = 0; i < n; ++i) {
    literal += std::string{CurrentChar()};
    ReadChar();
  }

  return Token(kind, literal);
}

Token Lexer::ComposeString() noexcept {
  auto literal = ReadLetters();
  return Token(TokenKind::STRING, literal);
}

std::string Lexer::ReadLetters() noexcept {
  auto begin = index;

  while (DoHaveLetter()) {
    ReadChar();
  }

  return std::string(std::begin(src) + begin, std::begin(src) + index);
}

void Lexer::SkipWhitespaces() noexcept {
  while (DoHaveWhitespace()) {
    ReadChar();
  }
}

bool Lexer::DoHaveWhitespace() const noexcept {
  return DoHave(' ') || DoHave('\t');
}

bool Lexer::DoHaveShortFlag() const noexcept {
  return DoHave('-') && !WillHave('-');
}

bool Lexer::DoHaveLongFlag() const noexcept {
  return DoHave('-') && WillHave('-');
}

bool Lexer::DoHaveLetter() const noexcept {
  auto curr = CurrentChar();
  return ('0' <= curr && curr <= '9') || ('a' <= curr && curr <= 'z') ||
         ('A' <= curr && curr <= 'Z');
}

bool Lexer::DoHave(char c) const noexcept { return CurrentChar() == c; }

bool Lexer::WillHave(char c) const noexcept { return NextChar() == c; }

char Lexer::CurrentChar() const noexcept {
  if (index >= src.size()) {
    return kCharEOF;
  }

  return src.at(index);
}

char Lexer::NextChar() const noexcept {
  auto next = index + 1;

  if (next >= src.size()) {
    return kCharEOF;
  }

  return src.at(next);
}

void Lexer::ReadChar() noexcept {
  if (index >= src.size()) {
    return;
  }

  ++index;
}
}  // namespace flags::analysis