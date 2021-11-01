#pragma once

#include "components/range.h"
#include "components/sigmoid.h"
#include "controls/knobs.h"

#include "rack.hpp"

#include <vector>

namespace dhe {
namespace xycloid {

struct ThrobSpeed {
  static auto constexpr range() -> Range { return Range{-10.F, 10.F}; }
  static auto constexpr taper() -> sigmoid::Taper {
    return sigmoid::s_taper_with_curvature(-0.8F);
  }

  static auto rotation_to_speed(float rotation) -> float {
    return range().scale(taper().apply(rotation));
  }

  static auto constexpr speed_to_rotation(float speed) -> float {
    return taper().invert(range().normalize(speed));
  }

  class Quantity : public rack::engine::ParamQuantity {
    auto getDisplayValue() -> float override {
      return rotation_to_speed(getValue());
    }

    void setDisplayValue(float speed) override {
      setValue(speed_to_rotation(speed));
    }
  };

  static void config(rack::engine::Module *module, int knob_id) {
    static auto constexpr initial_speed_hz{1.F};
    static auto constexpr initial_rotation =
        speed_to_rotation(initial_speed_hz);

    module->configParam<Quantity>(knob_id, 0.F, 1.F, initial_rotation, "Speed",
                                  " Hz");
  }
};

struct WobbleRatio {
  static auto constexpr max_ratio = 16.F;
  static auto constexpr inward_ratio_range = Range{0.F, -max_ratio};
  static auto constexpr outward_ratio_range = Range{0.F, max_ratio};
  static auto constexpr bidirectional_ratio_range =
      Range{-max_ratio, max_ratio};

  static inline auto ranges() -> std::vector<Range> const & {
    static auto const ranges = std::vector<Range>{
        inward_ratio_range, bidirectional_ratio_range, outward_ratio_range};
    return ranges;
  }

  class Quantity : public rack::engine::ParamQuantity {
  public:
    inline auto ratio(float rotation) -> float {
      auto const unquantized_ratio = range_.scale(rotation);
      return quantize_ ? std::round(unquantized_ratio) : unquantized_ratio;
    }

    auto getDisplayValue() -> float override { return ratio(getValue()); }

    void setDisplayValue(float bounce_ratio) override {
      auto const rotation = range_.normalize(bounce_ratio);
      setValue(rotation);
    }

    void select_range(int i) { range_ = ranges()[i]; }

    void quantize(bool b) { quantize_ = b; }

  private:
    Range range_{outward_ratio_range};
    bool quantize_{false};
  };

  static inline void config(rack::engine::Module *module, int knob_id,
                            std::string const &name) {
    module->configParam<Quantity>(knob_id, 0.F, 1.F, 0.5F, name, "x");
  }
};
} // namespace xycloid
} // namespace dhe
