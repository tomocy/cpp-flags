#ifndef TOMOCY_CPP_FLAGS_FLAGS_H
#define TOMOCY_CPP_FLAGS_FLAGS_H

#include <memory>
#include <string>

namespace flags {
class Value {
 public:
  virtual ~Value() {}
  virtual Value& operator=(const std::string& value) = 0;
  virtual std::string Type() const noexcept = 0;
  virtual std::string ToString() const noexcept = 0;
};
}  // namespace flags

namespace flags {
class String : public Value {
 public:
  static std::unique_ptr<String> Make(const std::string& value) noexcept;

  explicit String(const std::string& value) noexcept;

  Value& operator=(const std::string& value) noexcept override;

  std::string Type() const noexcept override;

  std::string ToString() const noexcept override;

 private:
  std::string value;
};
}  // namespace flags

namespace flags {
class Flag {
 public:
  using value_t = std::unique_ptr<Value>;

  Flag(const std::string& name, value_t value);

  Flag(const std::string& name, value_t value, const std::string& description);

  std::string Usage() const noexcept;

  const std::string& Name() const noexcept;

 private:
  const std::string& ValidateName(const std::string& name) const;
  value_t ValidateValue(value_t&& value) const;
  const std::string& ValidateDescription(const std::string& description) const;

  std::string name;
  value_t value;
  std::string description;
};
}  // namespace flags

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