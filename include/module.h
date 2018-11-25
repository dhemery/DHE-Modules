#pragma once

#include <engine.hpp>

#include "util/modulation.h"
#include "util/range.h"

namespace DHE {

struct Module : rack::Module {
  Module(int param_count, int input_count, int output_count)
      : rack::Module(param_count, input_count, output_count) {}

  auto modulated(int knob, int cv) const -> float {
    return Modulation::of(params[knob], inputs[cv]);
  }

  auto modulated(int knob, int cv, int av) const -> float {
    return Modulation::of(params[knob], inputs[cv], params[av]);
  }
};

} // namespace DHE
