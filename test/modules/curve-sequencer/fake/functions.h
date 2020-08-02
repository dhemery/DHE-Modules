#pragma once

#include <exception>

namespace test {
struct UnexpectedFunctionCall : public std::exception {
  UnexpectedFunctionCall(char const *name) : name{name} {}

  const char *what() const throw() { return name; }

private:
  char const *name;
};
} // namespace test
