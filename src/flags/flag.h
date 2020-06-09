#ifndef TOMOCY_CPP_FLAGS_FLAGS_FLAG_H
#define TOMOCY_CPP_FLAGS_FLAGS_FLAG_H

#include <memory>
#include <string>
#include <tuple>

#include "src/flags/values.h"

namespace flags {
class Flag {
 public:
  using value_t = std::unique_ptr<Value>;

  Flag() = default;

  Flag(const std::string& name, value_t value);

  Flag(const std::string& name, value_t value, const std::string& description);

  std::string Usage() const noexcept;

  const std::string& Name() const noexcept;

  void SetValue(const std::string& value);

  template <typename T>
  std::tuple<T, bool> TryToGet() const noexcept;

  template <typename T>
  T Get() const;

 private:
  const std::string& ValidateName(const std::string& name) const;
  value_t ValidateValue(value_t&& value) const;
  const std::string& ValidateDescription(const std::string& description) const;

  std::string name;
  value_t value;
  std::string description;
};
}  // namespace flags

#endif