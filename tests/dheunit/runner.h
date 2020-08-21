#pragma once
#include "test.h"

#include <algorithm>
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace dhe {
namespace unit {
namespace runner {

/**
 * A TestID identifies a test by suite name and test name.
 * A standalone test (not part of a suite) has an empty suite name.
 */
class TestID {
public:
  TestID(std::string suite_name, std::string test_name)
      : suite_name_{std::move(suite_name)}, test_name_{std::move(test_name)} {}

  auto suite_name() const -> std::string { return suite_name_; }

  auto test_name() const -> std::string { return test_name_; }

  auto operator<(TestID const &rhs) const -> bool {
    if (suite_name_ < rhs.suite_name_) {
      return true;
    }
    if (rhs.suite_name_ < suite_name_) {
      return false;
    }
    return test_name_ < rhs.test_name_;
  }

private:
  std::string const suite_name_;
  std::string const test_name_;
};

class TestResult {
public:
  TestResult(TestID id, bool passed, std::vector<std::string> log)
      : id_{std::move(id)}, passed_{passed}, log_{std::move(log)} {}
  auto id() const -> TestID const & { return id_; }
  auto passed() const -> bool { return passed_; }
  auto log() const -> std::vector<std::string> const & { return log_; }

private:
  TestID const id_;
  bool const passed_;
  std::vector<std::string> const log_;
};

class FailNowException : public std::exception {};

class Runner : public Tester {
public:
  Runner(TestID id, TestFunc test)
      : id_{std::move(id)}, test_{std::move(test)} {}

  void fail() override { passed_ = false; }

  void fail_now() override {
    fail();
    throw FailNowException{};
  }

  auto run() -> TestResult {
    try {
      test_(*this);
    } catch (FailNowException const &ignored) {
    } catch (char const *s) {
      error("Unexpected string exception: ", s);
    } catch (std::exception const &e) {
      error("Unexpected exception: ", e.what());
    } catch (...) {
      error("Unrecognized exception");
    }
    return TestResult{id_, passed_, log_};
  }

private:
  void add_log_entry(std::string entry) override { log_.push_back(entry); }

  TestID const id_;
  TestFunc test_;
  std::vector<std::string> log_;
  bool passed_{true};
};

template <typename C> class RunTest {
public:
  RunTest(C &controller) : controller_{controller} {}

  void operator()(std::pair<TestID, TestFunc> const &id_and_test) {
    auto const id{id_and_test.first};
    if (!controller_.select_test(id)) {
      return;
    }
    auto test{id_and_test.second};
    auto const result = Runner{id, test}.run();
    controller_.report(result);
  }

private:
  C &controller_;
};

template <typename C> class RunSuite {
public:
  RunSuite(C &controller) : controller_{controller} {}

  void operator()(std::pair<std::string, Suite *> const &id_and_suite) {
    auto const suite_id{id_and_suite.first};
    if (!controller_.select_suite(suite_id)) {
      return;
    }
    auto *suite{id_and_suite.second};

    std::vector<std::pair<TestID, TestFunc>> suite_tests{};
    auto registrar = [suite_id, &suite_tests](std::string const &test_name,
                                              TestFunc const &test) {
      const TestID test_id = TestID{suite_id, test_name};
      suite_tests.emplace_back(test_id, test);
    };
    suite->register_tests(registrar);

    std::for_each(suite_tests.cbegin(), suite_tests.cend(),
                  RunTest<C>{controller_});
  }

private:
  C &controller_;
};

class TestRun {
public:
  template <typename C> void run(C &controller) {
    std::for_each(suites_.cbegin(), suites_.cend(), RunSuite<C>{controller});
    std::for_each(tests_.cbegin(), tests_.cend(), RunTest<C>{controller});
  }

  void register_suite(std::string const &name, Suite *suite) {
    suites_[name] = suite;
  }

  void register_test(std::string const &name, Test *test) {
    auto const test_id = TestID{"", name};
    tests_[test_id] = [test](Tester &t) { test->run(t); };
  }

private:
  std::map<std::string, Suite *> suites_{};
  std::map<TestID, TestFunc> tests_{};
};

static auto test_run() -> TestRun & {
  static auto the_test_run = TestRun{};
  return the_test_run;
}

class StandardTestController {
public:
  auto select_suite(std::string const & /*name*/) -> bool { return true; }

  auto select_test(TestID const & /*id*/) -> bool { return true; }

  void report(TestResult const &result) {
    test_count_++;
    if (result.passed()) {
      std::cout << pass_text << "PASSED: ";
    } else {
      fail_count_++;
      std::cout << fail_text << "FAILED: ";
    }
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

  int test_count_ = 0;
  int fail_count_ = 0;
};

template <typename C> static inline auto run_tests(C &controller) -> int {
  test_run().run(controller);
  return controller.summarize();
}

} // namespace runner

Test::Test(std::string const &name) {
  runner::test_run().register_test(name, this);
}

Suite::Suite(std::string const &name) {
  runner::test_run().register_suite(name, this);
}
} // namespace unit
} // namespace dhe
