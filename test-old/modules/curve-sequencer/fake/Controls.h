#pragma once

#include "components/taper.h"
#include "modules/curve-sequencer/AdvanceMode.h"
#include "modules/curve-sequencer/GenerateMode.h"

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
  std::function<bool()> isGated{[]() -> bool { throw "Controls.isGated()"; }};
  std::function<bool()> isLooping{
      []() -> bool { throw "Controls.isLooping()"; }};
  std::function<bool()> isReset{[]() -> bool { throw "Controls.isReset()"; }};
  std::function<bool()> isRunning{
      []() -> bool { throw "Controls.isRunning()"; }};
  std::function<float()> getOutput{
      []() -> float { throw "Controls.output()"; }};
  std::function<void(float voltage)> setOutput{
      [](float f) { throw "Controls.output(voltage)"; }};
  std::function<int()> selectionStart{
      []() -> int { throw "Controls.selectionStart()"; }};
  std::function<int()> selectionLength{
      []() -> int { throw "Controls.isSelectionLength()"; }};

  auto output() const -> float { return getOutput(); }
  void output(float voltage) const { setOutput(voltage); };

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

  std::function<bool(int)> isEnabled{
      [](int s) -> bool { throw "Controls.isEnabled(step)"; }};
  std::function<void(int step, float progress)> showProgress{
      [](int s, float f) { throw "Controls.showProgress(step,progress)"; }};
  std::function<void(int step)> showInactive{
      [](int s) { throw "Controls.showInactive(step)"; }};
};

} // namespace fake
} // namespace curve_sequencer
} // namespace test
