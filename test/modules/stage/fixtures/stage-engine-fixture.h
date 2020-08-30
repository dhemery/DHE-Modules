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

template <typename EngineTest> auto test(EngineTest engine_test) -> TestFunc {
  return [engine_test](Tester &t) {
    Controls controls{};
    SimpleMode defer_mode{};
    SimpleMode input_mode{};
    TimedMode generate_mode{};
    SimpleMode level_mode{};
    StageEngine engine{controls, defer_mode, input_mode, generate_mode,
                       level_mode};
    engine_test(t, controls, defer_mode, input_mode, generate_mode, level_mode,
                engine);
  };
}

template <typename EngineTest>
auto in_input_mode(EngineTest engine_test) -> TestFunc {
  return [engine_test](Tester &t) {
    Controls controls{};
    SimpleMode defer_mode{};
    SimpleMode input_mode{};
    TimedMode generate_mode{};
    SimpleMode level_mode{};
    StageEngine engine{controls, defer_mode, input_mode, generate_mode,
                       level_mode};

    engine.process(0.F);
    input_mode = SimpleMode{};

    engine_test(t, controls, defer_mode, input_mode, generate_mode, level_mode,
                engine);
  };
}

template <typename EngineTest>
auto in_defer_mode(EngineTest engine_test) -> TestFunc {
  return [engine_test](Tester &t) {
    Controls controls{};
    SimpleMode defer_mode{};
    SimpleMode input_mode{};
    TimedMode generate_mode{};
    SimpleMode level_mode{};
    StageEngine engine{controls, defer_mode, input_mode, generate_mode,
                       level_mode};
    // Starts in input mode

    // input mode + defer rise -> defer mode
    controls.defer_ = true;
    engine.process(0.F);
    // leave defer high

    // Reset the fields of the modes we've traversed
    input_mode = SimpleMode{};
    defer_mode = SimpleMode{};

    engine_test(t, controls, defer_mode, input_mode, generate_mode, level_mode,
                engine);
  };
}

template <typename EngineTest>
auto in_generate_mode(EngineTest engine_test) -> TestFunc {
  return [engine_test](Tester &t) {
    Controls controls{};
    SimpleMode defer_mode{};
    SimpleMode input_mode{};
    TimedMode generate_mode{};
    SimpleMode level_mode{};
    StageEngine engine{controls, defer_mode, input_mode, generate_mode,
                       level_mode};
    // Starts in input mode

    // input mode + rising gate -> generate mode
    controls.gate_ = true;
    engine.process(0.F);
    controls.gate_ = false;

    // Reset the fields of the modes we've traversed
    input_mode = SimpleMode{};
    generate_mode = TimedMode{};

    engine_test(t, controls, defer_mode, input_mode, generate_mode, level_mode,
                engine);
  };
}

template <typename EngineTest>
auto in_level_mode(EngineTest engine_test) -> TestFunc {
  return [engine_test](Tester &t) {
    Controls controls{};
    SimpleMode defer_mode{};
    SimpleMode input_mode{};
    TimedMode generate_mode{};
    SimpleMode level_mode{};
    StageEngine engine{controls, defer_mode, input_mode, generate_mode,
                       level_mode};
    // Starts in input mode

    // input mode + gate rise -> generate mode
    controls.gate_ = true;
    engine.process(0.F);
    controls.gate_ = false;

    // generate mode + generate completes -> level mode
    generate_mode.event_ = Event::Completed;
    engine.process(0.F);

    // Reset the fields of the modes we've traversed
    input_mode = SimpleMode{};
    generate_mode = TimedMode{};
    level_mode = SimpleMode{};

    engine_test(t, controls, defer_mode, input_mode, generate_mode, level_mode,
                engine);
  };
}

} // namespace stage
} // namespace test
