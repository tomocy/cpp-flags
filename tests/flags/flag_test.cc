#include "external/gtest/googletest/include/gtest/gtest.h"
#include "src/flags/flags.h"

TEST(FlagUsage, Success) {
  auto flag = flags::Flag("name", flags::String::Make("value"),
                          "the description of the name");
  EXPECT_EQ("--name string  the description of the name (default: \"value\")",
            flag.Usage());
}

TEST(FlagUsage, FailedDueToUnspecifiedDescription) {
  auto flag = flags::Flag("name", flags::String::Make("value"));
  EXPECT_EQ("--name string  (default: \"value\")", flag.Usage());
}

TEST(FlagGet, Success) {
  EXPECT_NO_THROW(
      flags::Flag("a", flags::String::Make("1")).Get<std::string>());
  EXPECT_EQ(flags::Flag("a", flags::String::Make("1")).Get<std::string>(), "1");

  EXPECT_NO_THROW(flags::Flag("a", flags::Int::Make(1)).Get<int>());
  EXPECT_EQ(flags::Flag("a", flags::Int::Make(1)).Get<int>(), 1);

  EXPECT_NO_THROW(flags::Flag("a", flags::Bool::Make(false)).Get<bool>());
  EXPECT_FALSE(flags::Flag("a", flags::Bool::Make(false)).Get<bool>());
}

TEST(FlagGet, FailedDueToUncastable) {
  EXPECT_THROW(flags::Flag("a", flags::Bool::Make(true)).Get<std::string>(),
               flags::Exception);

  EXPECT_THROW(flags::Flag("a", flags::Bool::Make(true)).Get<int>(),
               flags::Exception);

  EXPECT_THROW(flags::Flag("a", flags::String::Make("a")).Get<bool>(),
               flags::Exception);
}

TEST(Flag, FailedDueToInvalidArgs) {
  EXPECT_THROW(flags::Flag("", flags::String::Make("value")), flags::Exception);
  EXPECT_THROW(flags::Flag("name", nullptr), flags::Exception);
  EXPECT_THROW(flags::Flag("", flags::String::Make("value")), flags::Exception);
  EXPECT_THROW(flags::Flag("name", nullptr), flags::Exception);
}