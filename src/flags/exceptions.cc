#include "src/flags/exceptions.h"

#include <string>

namespace flags {
Exception::Exception(const std::string& msg) noexcept : msg(msg) {}

const std::string& Exception::What() const noexcept { return msg; }
}  // namespace flags