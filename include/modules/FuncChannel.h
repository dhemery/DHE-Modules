#pragma once

#include "util/gain.h"
#include "util/signal.h"

namespace DHE {

static constexpr auto attenuation_range = Range{0.f, 1.f};
static constexpr auto invertible_attenuation_range = Range{-1.f, 1.f};
static constexpr auto invertible_gain_range = Range{-2.f, 2.f};

static constexpr auto half_bipolar_range = Range{0.f, 5.f};
static constexpr auto invertible_unipolar_range = Range{-10.f, 10.f};

class FuncChannel {
public:
  FuncChannel(rack::Module *module, int input, int amount_knob_param,
              int output);
  auto adjust(float upstream) -> float;
  void set_operator(bool is_multiplication);
  void set_addition_range(int selection);
  void set_multiplication_range(int selection);

private:
  const rack::Input &input_port;
  bool is_multiplication = false;
  Range const *addition_range{&Signal::bipolar_range};
  Range const *multiplication_range{&Gain::range};
  const float &amount;
  float &output;
};
} // namespace DHE
