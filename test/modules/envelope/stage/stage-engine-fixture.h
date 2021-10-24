#pragma once

#include "components/latch.h"
#include "modules/envelope/stage/engine.h"

#include "dheunit/test.h"

#include <functional>

namespace test {
namespace envelope {

using dhe::Latch;
using dhe::envelope::mode::Event;
using dhe::unit::Tester;
using TestFunc = std::function<void(Tester &)>;

struct Signals {
  auto defer() const -> bool { return defer_; }
  auto gate() const -> bool { return gate_; }
  void show_eoc(bool show) {
    eoc_ = show;
    showed_eoc_ = true;
  }
  bool defer_{};      // NOLINT
  bool gate_{};       // NOLINT
  bool showed_eoc_{}; // NOLINT
  bool eoc_{};        // NOLINT
};

struct SimpleMode {
  void enter() { entered_ = true; }
  void execute() { executed_ = true; }
  void exit() { exited_ = true; }
  bool entered_{};  // NOLINT
  bool executed_{}; // NOLINT
  bool exited_{};   // NOLINT
};

struct TimedMode {
  void enter() { entered_ = true; }
  void exit() { exited_ = true; }
  auto execute(Latch &latch, float sample_time) -> Event {
    executed_ = true;
    latch_ = latch;
    sample_time_ = sample_time;
    return event_;
  }
  bool entered_{};      // NOLINT
  bool executed_{};     // NOLINT
  bool exited_{};       // NOLINT
  Event event_{};       // NOLINT
  Latch latch_{};       // NOLINT
  float sample_time_{}; // NOLINT
};

using StageEngine =
    dhe::envelope::stage::StageEngine<Signals, SimpleMode, SimpleMode,
                                      TimedMode, SimpleMode>;

template <typename Prepare, typename Run>
auto test(Prepare prepare, Run run) -> TestFunc {
  return [prepare, run](Tester &t) {
    Signals signals{};
    SimpleMode defer_mode{};
    SimpleMode input_mode{};
    TimedMode generate_mode{};
    SimpleMode level_mode{};
    StageEngine engine{signals, defer_mode, input_mode, generate_mode,
                       level_mode};
    prepare(signals, defer_mode, input_mode, generate_mode, level_mode, engine);
    run(t, signals, defer_mode, input_mode, generate_mode, level_mode, engine);
  };
}
} // namespace envelope
} // namespace test
