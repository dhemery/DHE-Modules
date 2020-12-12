#pragma once

#include "components/latch.h"
#include "modules/stage/hostage-engine.h"
#include <dheunit/test.h>
#include <functional>

namespace test {
namespace stage {

using dhe::Latch;
using dhe::stage::Event;
using dhe::stage::Mode;
using dhe::unit::Tester;
using TestFunc = std::function<void(Tester &)>;

struct Controls {
  auto defer() const -> bool { return defer_; }
  auto gate() const -> bool { return gate_; }
  auto mode() const -> Mode { return mode_; }
  void show_eoc(bool show) {
    eoc_ = show;
    showed_eoc_ = true;
  }
  bool defer_{};      // NOLINT
  bool gate_{};       // NOLINT
  bool showed_eoc_{}; // NOLINT
  Mode mode_{};       // NOLINT
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

struct LatchedMode {
  void enter() { entered_ = true; }
  void exit() { exited_ = true; }
  auto execute(Latch &latch) -> Event {
    executed_ = true;
    latch_ = latch;
    return event_;
  }
  bool entered_{};  // NOLINT
  bool executed_{}; // NOLINT
  bool exited_{};   // NOLINT
  Event event_{};   // NOLINT
  Latch latch_{};   // NOLINT
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

using HostageEngine =
    dhe::stage::HostageEngine<Controls, SimpleMode, SimpleMode, TimedMode,
                              LatchedMode, SimpleMode>;

template <typename Prepare, typename Run>
auto test(Prepare prepare, Run run) -> TestFunc {
  return [prepare, run](Tester &t) {
    Controls controls{};
    SimpleMode input_mode{};
    SimpleMode defer_mode{};
    TimedMode hold_mode{};
    LatchedMode sustain_mode{};
    SimpleMode idle_mode{};
    HostageEngine engine{controls,  input_mode,   defer_mode,
                         hold_mode, sustain_mode, idle_mode};
    prepare(controls, input_mode, defer_mode, hold_mode, sustain_mode,
            idle_mode, engine);
    run(t, controls, input_mode, defer_mode, hold_mode, sustain_mode, idle_mode,
        engine);
  };
}
} // namespace stage
} // namespace test
