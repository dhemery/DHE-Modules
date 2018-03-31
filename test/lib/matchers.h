#pragma once

#include "lib/catch.hpp"

inline Catch::Matchers::Floating::WithinUlpsMatcher near(float expected, float proximity = 1) {
  return Catch::Matchers::WithinULP(expected, proximity);
}

