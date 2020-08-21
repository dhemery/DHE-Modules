#pragma once

#include "components/Taper.h"
#include "modules/stage/Mode.h"

#include <functional>

namespace test {
namespace stage {
using dhe::stage::Mode;
using dhe::taper::VariableTaper;

struct FakeControls {
  std::function<float()> curvature{
      []() -> float { throw "FakeControls.curvature()"; }};
  std::function<bool()> defer{[]() -> float { throw "FakeControls.defer()"; }};
  std::function<float()> duration{
      []() -> float { throw "FakeControls.duration()"; }};
  std::function<float()> input{[]() -> float { throw "FakeControls.input()"; }};
  std::function<bool()> gate{[]() -> float { throw "FakeControls.gate()"; }};
  std::function<float()> level{[]() -> float { throw "FakeControls.level()"; }};
  std::function<Mode()> mode{[]() -> Mode { throw "FakeControls.mode()"; }};
  std::function<void(float)> output{
      [](float v) { throw "FakeControls.output(voltage)"; }};
  std::function<void(bool)> showActive{
      [](bool a) { throw "FakeControls.showActive(active)"; }};
  std::function<void(bool)> showEoc{
      [](bool a) { throw "FakeControls.showEoc(eoc)"; }};
  std::function<VariableTaper const *()> taper{
      []() -> VariableTaper const * { throw "FakeControls.taper()"; }};
};
} // namespace stage
} // namespace test
