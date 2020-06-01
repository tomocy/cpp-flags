#include "src/flags.h"

#include <memory>

#include "external/gtest/googletest/include/gtest/gtest.h"

TEST(FlagUsage, DescriptionIsSpecified) {
  auto flag = flags::Flag("name", flags::String::Make("value"),
                          "the description of the name");
  EXPECT_EQ("--name string  the description of the name (default: \"value\")",
            flag.Usage());
}

TEST(FlagUsage, DescriptionIsNotSpecified) {
  auto flag = flags::Flag("name", flags::String::Make("value"));
  EXPECT_EQ("--name string  (default: \"value\")", flag.Usage());
}

TEST(Flag, InvalidArgs) {
  EXPECT_THROW(flags::Flag("", flags::String::Make("value")), flags::Exception);
  EXPECT_THROW(flags::Flag("name", nullptr), flags::Exception);
}