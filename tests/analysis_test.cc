#include "src/analysis.h"

#include <string>
#include <vector>

#include "external/gtest/googletest/include/gtest/gtest.h"

TEST(LexerReadToken, Success) {
  using namespace flags::analysis;

  auto src = std::string("-a --bb=ccc x -y z");
  auto expected = std::vector<Token>{
      kTokenShortFlag,
      Token(TokenKind::STRING, "a"),
      Token(TokenKind::WHITESPACE, " "),
      kTokenLongFlag,
      Token(TokenKind::STRING, "bb"),
      kTokenEqual,
      Token(TokenKind::STRING, "ccc"),
      Token(TokenKind::WHITESPACE, " "),
      Token(TokenKind::STRING, "x"),
      Token(TokenKind::WHITESPACE, " "),
      kTokenShortFlag,
      Token(TokenKind::STRING, "y"),
      Token(TokenKind::WHITESPACE, " "),
      Token(TokenKind::STRING, "z"),
      kTokenEOF,
  };

  auto lex = Lexer(std::vector<char>(std::begin(src), std::end(src)));

  for (auto expected : expected) {
    auto actual = lex.ReadToken();

    EXPECT_EQ(expected.Kind(), actual.Kind());
    EXPECT_EQ(expected.Literal(), actual.Literal());
  }
}