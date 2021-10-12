#include <dheunit/runner.h>

#include <iostream>
#include <string>

using dhe::unit::runner::run_tests;

auto main(int argc, char *argv[]) -> int {
  static auto const verbose_flag = std::string{"--verbose"};
  static auto const verbose_short_flag = std::string{"-v"};

  bool verbose = false;
  for (int i = 1; i < argc; i++) {
    if (verbose_flag == argv[i] || verbose_short_flag == argv[i]) {
      verbose = true;
    }
  }

  return run_tests(std::cout, verbose) ? 1 : 0;
}
