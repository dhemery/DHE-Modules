#pragma once

#include <functional>
#include <ios>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace dhe {
namespace unit {
/**
 * Builds a log entry from one or more write operations.
 */
class LogEntry {
public:
  struct FormatError : public std::runtime_error {
    FormatError(char const *what) : std::runtime_error{what} {}
  };

  LogEntry() { os_ << std::boolalpha; }

  template <typename T> void write(T &&t) { os_ << t; }

  template <typename T, typename... Ts> void write(T &&t, Ts &&... ts) {
    write(t);
    os_ << ' ';
    write(ts...);
  }

  void writef(char const *f) {
    if (f == nullptr) {
      throw FormatError{"Log format error: null format"};
    }
    while (f[0] != 0) {
      if (f[0] == '{' && f[1] == '}') {
        throw FormatError{"Log format error: not enough arguments"};
      }
      os_ << f[0];
      f++;
    }
  }

  template <typename T, typename... Ts>
  void writef(char const *f, T &&t, Ts &&... ts) {
    if (f == nullptr) {
      throw FormatError{"Log format error: null format"};
    }
    while (f[0] != 0) {
      if (f[0] == '{' && f[1] == '}') {
        os_ << t;
        return writef(f + 2, ts...);
      }
      os_ << f[0];
      f++;
    }
    throw FormatError{"Log format error: too many arguments"};
  }

  auto str() const -> std::string { return os_.str(); }

private:
  std::ostringstream os_{};
};

/**
 * Each test function receives a tester to report test failures and other
 * information. A test ends when it calls fatal() or failNow() or when it
 * returns.
 */
class Tester {
public:
  /**
   * Writes the string representation of each arg to the test's log, separated
   * by spaces.
   */
  template <typename... Ts> void log(Ts &&... args) {
    auto entry = LogEntry{};
    entry.write(args...);
    add_log_entry(entry.str());
  }

  /**
   * Equivalent to log(args) followed by fail().
   */
  template <typename... Ts> void error(Ts &&... args) {
    log(args...);
    fail();
  }

  /**
   * Equivalent to log(args) followed by failNow().
   */
  template <typename... Ts> void fatal(Ts &&... args) {
    log(args...);
    fail_now();
  }

  /**
   * Writes the format string to the test's log, replacing each {} with the
   * string representation of the corresponding arg.
   */
  template <typename... Ts> void logf(char const *format, Ts &&... args) {
    auto entry = LogEntry{};
    entry.writef(format, args...);
    add_log_entry(entry.str());
  };

  /**
   * Equivalent to logf(format, args) followed by fail().
   */
  template <typename... Ts> void errorf(char const *format, Ts &&... args) {
    logf(format, args...);
    fail();
  };

  /**
   * Equivalent to logf(format, args) followed by failNow().
   */
  template <typename... Ts> void fatalf(char const *format, Ts &&... args) {
    logf(format, args...);
    fail_now();
  };

  /**
   * Marks the test as failed and continues executing it.
   */
  virtual void fail() = 0;

  /**
   * Marks the test as failed and stops executing it.
   */
  virtual void fail_now() = 0;

protected:
  virtual void add_log_entry(std::string entry) = 0;
};

/**
 * A standalone test (not part of a suite).
 */
class Test {
public:
  /**
   * Constructs a test and registers it by name.
   */
  Test(std::string const &name);

  /**
   * Called by the test runner to execute this test.
   */
  virtual void run(Tester &tester) = 0;
};

/**
 * The type of object that can be executed as a test.
 */
using TestFunc = std::function<void(Tester &)>;

/**
 * The type of function that registers a TestFunc with its suite.
 * Each suite will passed a TestRegistrar that it can use to register its tests.
 */
using TestRegistrar =
    std::function<void(std::string const &, TestFunc const &)>;

/**
 * A suite of tests.
 */
class Suite {
public:
  /**
   * Constructs a test suite and registers it by name.
   */
  Suite(std::string const &name);

  /**
   * Called by the test runner to obtain the suite's tests. Your implementation
   * can call the registrar any number of times to submit tests for the suite to
   * run. The test runner prepends the suite's name to the name of each test.
   */
  virtual void register_tests(TestRegistrar) = 0;
};
} // namespace unit
} // namespace dhe
