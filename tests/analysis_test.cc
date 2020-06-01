#include "src/analysis.h"

#include <string>
#include <vector>

#include "external/gtest/googletest/include/gtest/gtest.h"

TEST(ReadToken, Empty) {
  using namespace flags::analysis;

  auto src = std::string("");
  auto expected = std::vector<Token>{
      kTokenEOF,
  };

  auto lex = Lexer(std::vector<char>(std::begin(src), std::end(src)));

  for (auto expected : expected) {
    auto actual = lex.ReadToken();
    EXPECT_EQ(actual.Kind(), expected.Kind());
    EXPECT_EQ(actual.Literal(), expected.Literal());
  }
}