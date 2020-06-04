# flags

![Test](https://github.com/tomocy/cpp-flags/workflows/Test/badge.svg?branch=master)

A library implements command-line flag parsing.

## Example

```cpp
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "external/flags/src/flags/flags.h"

int main(int n, const char** args) {
  auto flags = flags::FlagSet("program");

  flags.AddFlag(flags::Flag("name", flags::String::Make("")));
  flags.AddFlag(flags::Flag("age", flags::Int::Make(0)));
  flags.AddFlag(flags::Flag("admin", flags::Bool::Make(false), "add a user as admin"));

  auto trimmed = std::vector<std::string>(args + 1, args + n);

  try {
    flags.Parse(trimmed);
  } catch (const flags::Exception& e) {
    std::cerr << flags.Usage() << std::endl;
    std::cerr << e.What() << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "name: " << flags.GetFlag("name").Get<std::string>() << std::endl;
  std::cout << "age: " << flags.GetFlag("age").Get<int>() << std::endl;
  std::cout << "admin: " << flags.GetFlag("admin").Get<bool>() << std::endl;

  return EXIT_SUCCESS;
}
```

If you provided the following arguments to the code above,

```
./program --name alice --age 20 --admin
```

The output should be below.

```
name: alice
age: 20
admin: 1
```

You can see this example with description in [example.cc](example/example.cc).

## Requirements

- [Bazel](https://bazel.build/)
- a C++17 compliant compiler

## Install

Add the following code to your `WORKSPACE` file.

```python
http_archive(
    name = "flags",
    sha256 = "5da3ddda4ccb87285c3855a08c77475948b25f55804da02dd1151a583f2cfb7b",
    strip_prefix = "cpp-flags-1.2",
    url = "https://github.com/tomocy/cpp-flags/archive/v1.2.zip",
)
```

## Usage

Include `flags.h` as below.

```cpp
#include "external/flags/src/flags/flags.h"
```

Add `flags` library to `deps` in your target in your `BUILD` file.

```python
deps = [
  "@flags//src/flags",
],
```

## Contribution

1. Fork
2. Create a branch
3. Commit your changes
4. Rebase the branch of yours against the master branch of this repository
5. Run `bazel tests //...` to test
6. Create a pull request for the changes of yours

## Licenses
- [MIT License](LICENSE)
