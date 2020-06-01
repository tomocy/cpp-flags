#include "src/analysis.h"

#include <iostream>
#include <string>
#include <vector>

#include "external/gtest/googletest/include/gtest/gtest.h"

TEST(LexerReadToken, ReadToken) {
  using namespace flags::analysis;

  auto src = std::string("-a --b");
  auto expected = std::vector<Token>{
      kTokenShortFlag,
      Token(TokenKind::STRING, "a"),
      Token(TokenKind::WHITESPACE, " "),
      kTokenLongFlag,
      Token(TokenKind::STRING, "b"),
      kTokenEOF,
  };

  auto lex = Lexer(std::vector<char>(std::begin(src), std::end(src)));

  for (auto expected : expected) {
    auto actual = lex.ReadToken();

    std::cout << actual.Literal() << std::endl;

    EXPECT_EQ(expected.Kind(), actual.Kind());
    EXPECT_EQ(expected.Literal(), actual.Literal());
  }
}