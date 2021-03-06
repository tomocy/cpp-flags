#ifndef TOMOCY_CPP_FLAGS_FLAGS_FLAG_SET_H
#define TOMOCY_CPP_FLAGS_FLAGS_FLAG_SET_H

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "src/flags/flag.h"

namespace flags {
std::string GenerateDefaultFlagSetUsage(
    const std::string& name, const std::map<std::string, Flag>& flags);
}

namespace flags {
class FlagSet {
 public:
  using usage_generator_t = std::function<std::string(
      const std::string& name, const std::map<std::string, Flag>& flags)>;

  FlagSet(const std::string& name);

  void AddFlag(Flag&& flag);

  const Flag& GetFlag(const std::string& name) const;

  const std::vector<std::string>& Args() const noexcept;

  void Parse(const std::vector<std::string>& args);

  void Parse(const std::string& args);

  std::string Usage() const noexcept;

  std::string Usage(const usage_generator_t& generator) const noexcept;

  size_t FlagSize() const noexcept;

  size_t ArgSize() const noexcept;

 private:
  const std::string& ValidateName(const std::string& name) const;

  std::string name;
  std::map<std::string, Flag> flags;
  std::vector<std::string> args;
};
}  // namespace flags

#endif