#include "src/flags.h"

#include <memory>

#include "external/gtest/googletest/include/gtest/gtest.h"

TEST(FlagSetUsage, Usage) {
  auto flags = flags::FlagSet("program");

  flags.AddFlag(flags::Flag("c", flags::Bool::Make(false)));
  flags.AddFlag(
      flags::Flag("b", flags::String::Make("2"), "the description of b"));
  flags.AddFlag(flags::Flag("a", flags::String::Make("1")));

  EXPECT_EQ(R"(program
  --a string  (default: "1")
  --b string  the description of b (default: "2")
  --c bool  (default: false))",
            flags.Usage());
}

TEST(FlagSetAddFlag, FlagIsAlreadyAdded) {
  auto flags = flags::FlagSet("program");

  EXPECT_NO_THROW(flags.AddFlag(flags::Flag("a", flags::String::Make("1"))));
  EXPECT_THROW(flags.AddFlag(flags::Flag("a", flags::String::Make("1"))),
               flags::Exception);
}

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

TEST(FlagCast, Castable) {
  EXPECT_NO_THROW(
      flags::Flag("a", flags::String::Make("1")).Get<std::string>());
  EXPECT_EQ(flags::Flag("a", flags::String::Make("1")).Get<std::string>(), "1");

  EXPECT_NO_THROW(flags::Flag("a", flags::Bool::Make(false)).Get<bool>());
  EXPECT_FALSE(flags::Flag("a", flags::Bool::Make(false)).Get<bool>());
}

TEST(FlagCast, Uncastable) {
  EXPECT_THROW(flags::Flag("a", flags::Bool::Make(true)).Get<std::string>(),
               flags::Exception);

  EXPECT_THROW(flags::Flag("a", flags::String::Make("a")).Get<bool>(),
               flags::Exception);
}

TEST(Flag, InvalidArgs) {
  EXPECT_THROW(flags::Flag("", flags::String::Make("value")), flags::Exception);
  EXPECT_THROW(flags::Flag("name", nullptr), flags::Exception);
}