#include "dheunit/runner.h"

using dhe::unit::runner::run_tests;
auto main() -> int { return run_tests(std::cout) ? 1 : 0; }
