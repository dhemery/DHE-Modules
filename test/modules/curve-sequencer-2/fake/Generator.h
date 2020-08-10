#pragma once

#include "helpers/fake-controls.h"

#include <string>

namespace test {
namespace fake {

  class Generator {
  public:
    std::function<bool(int, float)> generate{
        [](int s, float p) -> bool { throw forbidden("Generator.generate", s, p); }};
  };
} // namespace fake
} // namespace test
