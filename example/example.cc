#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "src/flags/flags.h"

int main(int n, const char** args) {
  auto flags = flags::FlagSet("program");

  // std::string, int, bool can be the value of Flag

  // Flag with the value of std::string
  // "name" is the name of this flag.
  // "" is the default value of this flag.
  flags.AddFlag(flags::Flag("name", flags::String::Make("")));

  // Flag with the value of int
  // "age" is the name of this flag.
  // 0 is the default value of this flag.
  flags.AddFlag(flags::Flag("age", flags::Int::Make(0)));

  // Flag with the value of bool
  // "adimin" is the name of this flag.
  // false is the default value of this flag.
  // You can put the description to this flag.
  flags.AddFlag(
      flags::Flag("admin", flags::Bool::Make(false), "add a user as admin"));

  // The program name should be trimmed.
  auto trimmed = std::vector<std::string>(args + 1, args + n);

  // flags::Exception is thrown if the parsing is failed.
  try {
    flags.Parse(trimmed);
  } catch (const flags::Exception& e) {
    // FlagSet generates its usage based on the added flags.
    std::cout << flags.Usage() << std::endl;

    std::cerr << e.What() << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "name: " << flags.GetFlag("name").Get<std::string>()
            << std::endl;
  std::cout << "age: " << flags.GetFlag("age").Get<int>() << std::endl;
  std::cout << "is admin: " << flags.GetFlag("admin").Get<bool>() << std::endl;

  // You can get the rest arguments but the flags.
  auto rest_args = flags.Args();

  std::cout << "args:" << std::endl;
  for (auto arg : rest_args) {
    std::cout << arg << std::endl;
  }

  return EXIT_SUCCESS;
}