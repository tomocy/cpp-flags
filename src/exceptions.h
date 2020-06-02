#ifndef TOMOCY_CPP_FLAGS_EXCEPTIONS_H
#define TOMOCY_CPP_FLAGS_EXCEPTIONS_H

#include <string>

namespace flags {
class Exception {
 public:
  explicit Exception(const std::string& msg) noexcept;

  const std::string& What() const noexcept;

 private:
  std::string msg;
};
}  // namespace flags

#endif