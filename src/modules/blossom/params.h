#pragma once

#include "components/range.h"
#include "components/sigmoid.h"
#include "controls/knobs.h"
#include "params/presets.h"

#include "rack.hpp"

#include <string>
#include <vector>

namespace dhe {
namespace blossom {

struct BounceRatio {
  static auto constexpr range() -> Range { return Range{1.F, 17.F}; }

  static inline auto value(float rotation, bool quantize) -> float {
    auto const ratio = range().scale(rotation);
    return quantize ? std::round(ratio) : ratio;
  }

  static inline auto rotation(float ratio) -> float {
    return range().normalize(ratio);
  }
};

struct BounceRatioQuantity : public rack::engine::ParamQuantity {
  auto getDisplayValue() -> float override {
    return BounceRatio::value(getValue(), quantize_);
  }

  void setDisplayValue(float ratio) override {
    auto const rotation = BounceRatio::rotation(ratio);
    setValue(rotation);
  }

  void set_quantize(bool quantize) { quantize_ = quantize; }

private:
  bool quantize_{false};
};

struct BounceRatioKnob {
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, float rotation = 0.5)
      -> BounceRatioQuantity * {
    return module->configParam<BounceRatioQuantity>(id, 0.F, 1.F, rotation,
                                                    name, "x");
  }
};

struct BounceRatioModes {
  enum Index { Quantized, Free };
  using ValueType = Index;

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{"Quantized", "Free"};
    return labels;
  }
};

struct SpinSpeed {
  static auto constexpr range() -> Range { return Range{-10.F, 10.F}; }
  static auto constexpr taper() -> sigmoid::Taper {
    return sigmoid::s_taper_with_curvature(-0.8F);
  }
  static auto constexpr rotation(float spin) -> float {
    return taper().invert(range().normalize(spin));
  }
  static auto constexpr value(float rotation) -> float {
    return range().scale(taper().apply(rotation));
  }
};

struct SpinSpeedQuantity : public rack::engine::ParamQuantity {
  auto getDisplayValue() -> float override {
    return SpinSpeed::value(getValue());
  }

  void setDisplayValue(float spin) override {
    setValue(SpinSpeed::rotation(spin));
  }
};

struct SpinSpeedKnob {
  static auto constexpr initial_spin_hz{1.F};
  static auto constexpr initial_rotation = SpinSpeed::rotation(initial_spin_hz);

  static inline auto config(rack::engine::Module *module, int knob_id,
                            std::string const &name,
                            float rotation = initial_rotation)
      -> SpinSpeedQuantity * {
    return module->configParam<SpinSpeedQuantity>(knob_id, 0.F, 1.F, rotation,
                                                  name, " Hz");
  }
};

} // namespace blossom
} // namespace dhe
