#pragma once

#include "components/latch.h"
#include "modules/stage/stage-engine.h"
#include <dheunit/test.h>

namespace test {
namespace stage {

using dhe::Latch;
using dhe::stage::Event;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

struct Controls {
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

using StageEngine = dhe::stage::StageEngine<Controls, SimpleMode, SimpleMode,
                                            TimedMode, SimpleMode>;

template <typename Run> auto test(Run run) -> TestFunc {
  return [run](Tester &t) {
    Controls controls{};
    SimpleMode defer_mode{};
    SimpleMode input_mode{};
    TimedMode generate_mode{};
    SimpleMode level_mode{};
    StageEngine engine{controls, defer_mode, input_mode, generate_mode,
                       level_mode};
    run(t, controls, defer_mode, input_mode, generate_mode, level_mode, engine);
  };
}
template <typename Prepare, typename Run>
auto test(Prepare prepare, Run run) -> TestFunc {
  return [prepare, run](Tester &t) {
    Controls controls{};
    SimpleMode defer_mode{};
    SimpleMode input_mode{};
    TimedMode generate_mode{};
    SimpleMode level_mode{};
    StageEngine engine{controls, defer_mode, input_mode, generate_mode,
                       level_mode};
    prepare(controls, defer_mode, input_mode, generate_mode, level_mode,
            engine);
    run(t, controls, defer_mode, input_mode, generate_mode, level_mode, engine);
  };
}
} // namespace stage
} // namespace test
