#include "dheunit/runner.h"

using dhe::unit::runner::run_tests;
using dhe::unit::runner::StandardTestController;

auto main() -> int {
  auto controller = StandardTestController{};
  return run_tests(controller);
}
