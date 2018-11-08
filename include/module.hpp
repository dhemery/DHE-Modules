#pragma once

#include <engine.hpp>

#include "util/range.hpp"

namespace DHE {

struct Module : rack::Module {
  Module(int param_count, int input_count, int output_count)
      : rack::Module(param_count, input_count, output_count) {}

  float gate_button(int index) const;

  float input(int index) const { return inputs[index].value; }

  float param(int index) const { return params[index].value; }

  std::function<float()> knob(int rotation) const;

  std::function<float()> knob(int rotation, int cv) const;

  std::function<float()> knob(int rotation, int cv, int av) const;

  std::function<const Range &()> range_switch(int switch_param) const;
};

} // namespace DHE
