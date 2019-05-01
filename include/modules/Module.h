#pragma once

#include "engine/Module.hpp"

namespace DHE {
class Module : public rack::engine::Module {
public:
  Module(int param_count, int input_count, int output_count);

  auto modulated(int knob_param, int cv_input) const -> float;

  auto modulated(int knob_param, int cv_input, int av_param) const -> float;
};
} // namespace DHE
