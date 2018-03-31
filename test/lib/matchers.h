#pragma once

#include "lib/catch.hpp"

inline Catch::Matchers::Floating::WithinUlpsMatcher near(float expected) {
  return Catch::Matchers::WithinULP(expected, 1);
}

