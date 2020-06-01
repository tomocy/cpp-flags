#include "src/flags.h"

#include <memory>

#include "external/gtest/googletest/include/gtest/gtest.h"

TEST(FlagUsage, Usage) {
  auto flag = flags::Flag("name", std::make_unique<flags::String>("value"),
                          "the description of the name");
  EXPECT_EQ("--name string  the description of the name (default: \"value\")",
            flag.Usage());
}

TEST(Flag, InvalidArgs) {
  EXPECT_THROW(flags::Flag("", std::make_unique<flags::String>("value")),
               flags::Exception);
  EXPECT_THROW(flags::Flag("name", nullptr), flags::Exception);
}