#pragma once

#include "lib/catch.hpp"

inline Catch::Matchers::Floating::WithinUlpsMatcher near(float expected, int proximity = 1) {
  return Catch::Matchers::WithinULP(expected, proximity);
}

