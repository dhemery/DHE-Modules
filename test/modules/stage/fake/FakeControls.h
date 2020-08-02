#pragma once

#include <functional>

namespace test {
namespace stage {
  struct FakeControls {
    std::function<float()> input{[]() -> float { throw "FakeControls.input()"; }};
    std::function<void(bool)> showActive{[](bool a) { throw "FakeControls.showActive(active)"; }};
    std::function<void(float)> output{[](float v) { throw "FakeControls.output(voltage)"; }};
  };
} // namespace stage
} // namespace test
