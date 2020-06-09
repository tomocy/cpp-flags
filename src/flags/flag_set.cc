#include "src/flags/flag_set.h"

#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "src/flags/exceptions.h"
#include "src/flags/flag.h"
#include "src/flags/parser.h"

namespace flags {
std::string GenerateDefaultFlagSetUsage(
    const std::string& name, const std::map<std::string, Flag>& flags) {
  auto usage = name + "\n";

  for (auto iter = std::begin(flags); iter != std::end(flags); ++iter) {
    usage += "  " + iter->second.Usage() + "\n";
  }

  return usage.erase(usage.size() - 1, 1);
}
}  // namespace flags

namespace flags {
FlagSet::FlagSet(const std::string& name) : name(ValidateName(name)) {}

void FlagSet::AddFlag(Flag&& flag) {
  if (flags.find(flag.Name()) != flags.end()) {
    throw Exception("flag \"" + flag.Name() + "\" is already added");
  }

  flags[flag.Name()] = std::move(flag);
}

const Flag& FlagSet::GetFlag(const std::string& name) const {
  if (flags.find(name) == flags.end()) {
    throw Exception("flag \"" + name + "\" is not added");
  }

  return flags.at(name);
}

const std::vector<std::string>& FlagSet::Args() const noexcept { return args; }

void FlagSet::Parse(const std::vector<std::string>& args) {
  std::ostringstream joined;
  std::copy(std::begin(args), std::end(args),
            std::ostream_iterator<std::string>(joined, " "));

  auto src = joined.str();

  if (!src.empty()) {
    src = src.erase(src.size() - 1, 1);
  }

  Parse(src);
}

void FlagSet::Parse(const std::string& args) {
  auto parser = Parser(std::vector<char>(std::begin(args), std::end(args)),
                       flags, this->args);

  parser.Parse();
}

std::string FlagSet::Usage() const noexcept {
  return Usage(GenerateDefaultFlagSetUsage);
}

std::string FlagSet::Usage(const usage_generator_t& generator) const noexcept {
  return generator(name, flags);
}

size_t FlagSet::FlagSize() const noexcept { return flags.size(); }

size_t FlagSet::ArgSize() const noexcept { return args.size(); }

const std::string& FlagSet::ValidateName(const std::string& name) const {
  if (name.empty()) {
    throw Exception("name should not be empty");
  }

  return name;
}
}  // namespace flags