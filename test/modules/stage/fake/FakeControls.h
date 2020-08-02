#pragma once

#include <functional>

namespace test {
namespace stage {
  struct FakeControls {
    std::function<float()> duration{[]() -> float { throw "FakeControls.duration()"; }};
    std::function<float()> input{[]() -> float { throw "FakeControls.input()"; }};
    std::function<float()> level{[]() -> float { throw "FakeControls.level()"; }};
    std::function<void(float)> output{[](float v) { throw "FakeControls.output(voltage)"; }};
    std::function<void(bool)> showActive{[](bool a) { throw "FakeControls.showActive(active)"; }};
  };
} // namespace stage
} // namespace test
