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

struct BounceRatioModes {
  enum Selection { Quantized, Free };

  static auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{"Quantized", "Free"};
    return labels;
  }
};

struct BounceRatio {
  struct KnobMapper {
    auto to_display_value(float rotation) const -> float {
      return ratio(rotation, quantize_);
    }

    auto to_rotation(float ratio) const -> float { return rotation(ratio); }

    void select_mode(BounceRatioModes::Selection selection) {
      quantize_ = selection == BounceRatioModes::Quantized;
    }

  private:
    bool quantize_{false};
  };

  static auto constexpr unit = "x";

  static auto ratio(float rotation, bool quantize) -> float {
    auto const ratio = range.scale(rotation);
    return quantize ? std::round(ratio) : ratio;
  }

  static auto constexpr rotation(float ratio) -> float {
    return range.normalize(ratio);
  }

private:
  static constexpr auto range = Range{1.F, 17.F};
};

struct SpinSpeed {
  static auto constexpr unit = " Hz";

  static auto rotation(float hertz) -> float {
    return taper().invert(range().normalize(hertz));
  }

  static auto hertz(float rotation) -> float {
    return range().scale(taper().apply(rotation));
  }

  struct KnobMapper {
    auto to_display_value(float rotation) const -> float {
      return hertz(rotation);
    }

    auto to_rotation(float hertz) const -> float { return rotation(hertz); }
  };

private:
  static auto constexpr range() -> Range { return Range{-10.F, 10.F}; }
  static auto taper() -> sigmoid::Taper const & {
    static auto const taper = sigmoid::s_taper_with_curvature(-0.8F);
    return taper;
  }
};

} // namespace blossom
} // namespace dhe
