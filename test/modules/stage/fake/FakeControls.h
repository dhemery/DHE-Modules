#pragma once

#include "components/Taper.h"

#include <functional>

namespace test {
namespace stage {
  using dhe::taper::VariableTaper;
  struct FakeControls {
    std::function<float()> curvature{[]() -> float { throw "FakeControls.curvature()"; }};
    std::function<float()> duration{[]() -> float { throw "FakeControls.duration()"; }};
    std::function<float()> input{[]() -> float { throw "FakeControls.input()"; }};
    std::function<float()> level{[]() -> float { throw "FakeControls.level()"; }};
    std::function<void(float)> output{[](float v) { throw "FakeControls.output(voltage)"; }};
    std::function<void(bool)> showActive{[](bool a) { throw "FakeControls.showActive(active)"; }};
    std::function<VariableTaper const *()> taper{[]() -> VariableTaper const * { throw "FakeControls.taper()"; }};
  };
} // namespace stage
} // namespace test
