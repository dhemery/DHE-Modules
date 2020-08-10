#pragma once

#include "helpers/fake-controls.h"

#include <string>

namespace test {
namespace fake {

  class Generator {
  public:
    std::function<void(int)> start = [](int s) { throw forbidden("start", s); };
    std::function<bool(float)> generate{[](float t) -> bool { throw forbidden("generate", t); }};
    std::function<void()> stop = []() { throw "stop"; };
  };
} // namespace fake
} // namespace test
