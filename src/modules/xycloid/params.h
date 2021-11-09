#pragma once

#include "components/range.h"
#include "components/sigmoid.h"
#include "controls/knobs.h"

#include "rack.hpp"

#include <vector>

namespace dhe {
namespace xycloid {

struct WobbleRatios {
  using PositionType = int;
  using ItemType = Range;

  enum { Inward, InwardOutward, Outward };

  static inline auto items() -> std::vector<ItemType> const & {
    static auto constexpr max_ratio = 16.F;
    static auto const ranges = std::vector<ItemType>{
        Range{0.F, -max_ratio}, Range{-max_ratio, max_ratio},
        Range{0.F, max_ratio}};
    return ranges;
  }

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"In", "-In +Out", "Out"};
    return labels;
  }

  static inline auto select(int selection) -> ItemType const & {
    return items()[selection];
  }

  static inline auto value(float rotation, int range, bool quantize) -> float {
    auto const ratio = select(range).scale(rotation);
    return quantize ? std::round(ratio) : ratio;
  }
};

struct WobbleRatioModes {
  using PositionType = int;

  enum { Quantized, Free };

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{"Quantized", "Free"};
    return labels;
  }
};

struct WobbleRatioQuantity : public rack::engine::ParamQuantity {
public:
  auto getDisplayValue() -> float override {
    return WobbleRatios::value(getValue(), range_selection_, quantize_);
  }

  void setDisplayValue(float value) override {
    auto const rotation =
        WobbleRatios::select(range_selection_).normalize(value);
    setValue(rotation);
  }

  void set_range_selection(int selection) { range_selection_ = selection; }

  void set_quantize(bool quantize) { quantize_ = quantize; }

private:
  int range_selection_{WobbleRatios::Outward};
  bool quantize_{false};
};

struct WobbleRatioKnob {
  static inline auto config(rack::engine::Module *module, int knob_id,
                            std::string const &name, float rotation = 0.5F)
      -> WobbleRatioQuantity * {
    return module->configParam<WobbleRatioQuantity>(knob_id, 0.F, 1.F, rotation,
                                                    name, "x");
  }
};

struct ThrobSpeed {
  static auto constexpr unit = " Hz";
  static auto constexpr range() -> Range { return Range{-10.F, 10.F}; }
  static auto constexpr taper() -> sigmoid::Taper {
    return sigmoid::s_taper_with_curvature(-0.8F);
  }

  static auto value(float rotation) -> float {
    return range().scale(taper().apply(rotation));
  }

  static auto constexpr rotation(float speed) -> float {
    return taper().invert(range().normalize(speed));
  }
};

struct ThrobSpeedQuantity : public rack::engine::ParamQuantity {
  auto getDisplayValue() -> float override {
    return ThrobSpeed::value(getValue());
  }

  void setDisplayValue(float speed) override {
    setValue(ThrobSpeed::rotation(speed));
  }
};

struct ThrobSpeedKnob {
  static auto constexpr default_speed_hz{1.F};
  static auto constexpr default_rotation =
      ThrobSpeed::rotation(default_speed_hz);

  static inline auto config(rack::engine::Module *module, int knob_id,
                            std::string const &name,
                            float rotation = default_rotation)
      -> ThrobSpeedQuantity * {
    return module->configParam<ThrobSpeedQuantity>(knob_id, 0.F, 1.F, rotation,
                                                   name, ThrobSpeed::unit);
  }
};
} // namespace xycloid
} // namespace dhe
