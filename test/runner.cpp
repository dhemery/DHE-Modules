#include "dheunit/runner.h"

namespace test {

using dhe::unit::runner::run_tests;
using dhe::unit::runner::TestID;
using dhe::unit::runner::TestResult;

class CustomRunner {
public:
  auto run() -> int { return run_tests(*this); }

  // Select every suite
  auto select_suite(std::string const & /*name*/) -> bool { return true; }

  // Select every test
  auto select_test(TestID const & /*id*/) -> bool { return true; }

  // Report only failures
  void report(TestResult const &result) {
    test_count_++;
    if (result.passed()) {
      return;
    }
    fail_count_++;
    std::cout << fail_text << "FAILED: ";
    std::cout << normal_text << result.id().suite_name() << ": "
              << result.id().test_name() << std::endl;
    for (auto const &entry : result.log()) {
      std::cout << "    " << entry << std::endl;
    }
  }

  auto summarize() -> int {
    std::cout << test_count_ << " tests";
    auto const pass_count = test_count_ - fail_count_;
    if (pass_count > 0) {
      std::cout << " / " << pass_text << test_count_ << " PASSED"
                << normal_text;
    }
    if (fail_count_ > 0) {
      std::cout << " / " << fail_text << fail_count_ << " FAILED"
                << normal_text;
    }
    std::cout << std::endl;
    return fail_count_ > 0 ? 1 : 0;
  }

private:
  static auto constexpr *fail_text = "\u001b[31m";
  static auto constexpr *pass_text = "\u001b[32m";
  static auto constexpr *normal_text = "\u001b[0m";

  int fail_count_{};
  int test_count_{};
};

} // namespace test

auto main() -> int { return test::CustomRunner{}.run(); }
