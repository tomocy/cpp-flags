#ifndef TOMOCY_CPP_FLAGS_VALUES_H
#define TOMOCY_CPP_FLAGS_VALUES_H

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

  const std::string& Value() const noexcept;

 private:
  std::string value;
};
}  // namespace flags

namespace flags {
class Int : public Value {
 public:
  static std::unique_ptr<Int> Make(int value) noexcept;

  explicit Int(int value) noexcept;

  Value& operator=(const std::string& value) override;

  std::string Type() const noexcept override;

  std::string ToString() const noexcept override;

  int Value() const noexcept;

 private:
  int value;
};
}  // namespace flags

namespace flags {
class Bool : public Value {
 public:
  static std::unique_ptr<Bool> Make(bool value) noexcept;

  explicit Bool(bool value) noexcept;

  Value& operator=(const std::string& value) override;

  std::string Type() const noexcept override;

  std::string ToString() const noexcept override;

  bool Value() const noexcept;

 private:
  bool value;
};
}  // namespace flags

#endif