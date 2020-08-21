#pragma once

#include "components/taper.h"
#include "modules/curve-sequencer/advance-mode.h"
#include "modules/curve-sequencer/generate-mode.h"

#include <functional>

namespace test {
namespace curve_sequencer {
namespace fake {
using dhe::curve_sequencer::AdvanceMode;
using dhe::curve_sequencer::GenerateMode;
using dhe::taper::VariableTaper;

struct Controls {
  // Sequencer controls
  std::function<float()> input{[]() -> float { throw "Controls.input()"; }};
  std::function<bool()> is_gated{[]() -> bool { throw "Controls.is_gated()"; }};
  std::function<bool()> is_looping{
      []() -> bool { throw "Controls.is_looping()"; }};
  std::function<bool()> is_reset{[]() -> bool { throw "Controls.is_reset()"; }};
  std::function<bool()> is_running{
      []() -> bool { throw "Controls.is_running()"; }};
  std::function<float()> get_output{
      []() -> float { throw "Controls.output()"; }};
  std::function<void(float voltage)> set_output{
      [](float f) { throw "Controls.output(voltage)"; }};
  std::function<int()> selection_start{
      []() -> int { throw "Controls.selection_start()"; }};
  std::function<int()> selection_length{
      []() -> int { throw "Controls.selection_length()"; }};

  auto output() const -> float { return get_output(); }
  void output(float voltage) const { set_output(voltage); };

  // Step controls
  std::function<GenerateMode(int step)> mode{
      [](int s) -> GenerateMode { throw "Controls.mode(step)"; }};
  std::function<AdvanceMode(int step)> condition{
      [](int s) -> AdvanceMode { throw "Controls.condition(step)"; }};

  std::function<float(int step)> curvature{
      [](int s) -> float { throw "Controls.curvature(step)"; }};
  std::function<float(int step)> duration{
      [](int s) -> float { throw "Controls.duration(step)"; }};
  std::function<float(int step)> level{
      [](int s) -> float { throw "Controls.level(step)"; }};
  std::function<VariableTaper const *(int step)> taper{
      [](int s) -> VariableTaper const * { throw "Controls.taper(step)"; }};

  std::function<bool(int)> is_enabled{
      [](int s) -> bool { throw "Controls.is_enabled(step)"; }};
  std::function<void(int step, float progress)> show_progress{
      [](int s, float f) { throw "Controls.show_progress(step,progress)"; }};
  std::function<void(int step)> show_inactive{
      [](int s) { throw "Controls.show_inactive(step)"; }};
};

} // namespace fake
} // namespace curve_sequencer
} // namespace test
