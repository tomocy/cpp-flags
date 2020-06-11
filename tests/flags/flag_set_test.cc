#include <memory>
#include <string>
#include <vector>

#include "external/gtest/googletest/include/gtest/gtest.h"
#include "src/flags/flags.h"

TEST(FlagSetAddFlag, Success) {
  auto flags = flags::FlagSet("program");

  EXPECT_NO_THROW(flags.AddFlag(flags::Flag("a", flags::String::Make("1"))));
}

TEST(FlagSetAddFlag, FailedDueToAlreadyAddedFlag) {
  auto flags = flags::FlagSet("program");

  flags.AddFlag(flags::Flag("a", flags::String::Make("1")));

  EXPECT_THROW(flags.AddFlag(flags::Flag("a", flags::String::Make("1"))),
               flags::Exception);
}

TEST(FlagSetFlag, Success) {
  auto flags = flags::FlagSet("program");

  flags.AddFlag(flags::Flag("a", flags::String::Make("1")));

  EXPECT_EQ("1", flags.GetFlag("a").Get<std::string>());
}

TEST(FlagSetFlag, FailedDueToUnadddedFlag) {
  auto flags = flags::FlagSet("program");

  EXPECT_THROW(flags.GetFlag("a").Get<std::string>(), flags::Exception);
}

TEST(FlagSetParse, Success) {
  auto flags = flags::FlagSet("program");

  flags.AddFlag(flags::Flag("c", flags::Bool::Make(false)));
  flags.AddFlag(
      flags::Flag("b", flags::String::Make("2"), "the description of b"));
  flags.AddFlag(flags::Flag("a", flags::String::Make("1")));

  auto args = std::vector<std::string>{"--a", "22", "--c",   "--b=333",
                                       "x",   "-y", "01234", "--z=56789"};

  flags.Parse(args);

  EXPECT_EQ("22", flags.GetFlag("a").Get<std::string>());
  EXPECT_EQ("333", flags.GetFlag("b").Get<std::string>());
  EXPECT_TRUE(flags.GetFlag("c").Get<bool>());
  auto expected_args =
      std::vector<std::string>{"x", "-y", "01234", "--z=56789"};
  EXPECT_EQ(expected_args, flags.Args());
}

TEST(FlagSetParse, SuccessWithEmptySource) {
  auto flags = flags::FlagSet("program");

  flags.AddFlag(flags::Flag("c", flags::Bool::Make(false)));
  flags.AddFlag(
      flags::Flag("b", flags::String::Make("2"), "the description of b"));
  flags.AddFlag(flags::Flag("a", flags::String::Make("1")));

  auto args = std::vector<std::string>{};

  EXPECT_NO_THROW(flags.Parse(args));
}

TEST(FlagSetParse, FailedDueToUngivenValue) {
  auto flags = flags::FlagSet("program");

  flags.AddFlag(flags::Flag("a", flags::String::Make("1")));

  auto args = std::vector<std::string>{"--a"};

  EXPECT_THROW(flags.Parse(args), flags::Exception);
}

TEST(FlagSetParse, FailedDueToUnknownFlag) {
  auto flags = flags::FlagSet("program");

  flags.AddFlag(flags::Flag("a", flags::String::Make("1")));

  auto args = std::vector<std::string>{"--b"};

  EXPECT_THROW(flags.Parse(args), flags::Exception);
}

TEST(FlagSetUsage, Success) {
  auto flags = flags::FlagSet("program");

  flags.AddFlag(flags::Flag("c", flags::Bool::Make(false)));
  flags.AddFlag(
      flags::Flag("b", flags::String::Make("2"), "the description of b"));
  flags.AddFlag(flags::Flag("a", flags::Int::Make(1)));

  EXPECT_EQ(R"(program
  --a int  (default: 1)
  --b string  the description of b (default: "2")
  --c bool  (default: false))",
            flags.Usage());
}

TEST(FlagSetFlagSize, Success) {
  auto flags = flags::FlagSet("program");

  EXPECT_EQ(0, flags.FlagSize());

  flags.AddFlag(flags::Flag("a", flags::Bool::Make(false)));

  EXPECT_EQ(1, flags.FlagSize());
}

TEST(FlagSetArgSize, Success) {
  auto flags = flags::FlagSet("program");

  EXPECT_EQ(0, flags.ArgSize());

  flags.Parse("a b c");

  EXPECT_EQ(3, flags.ArgSize());
}