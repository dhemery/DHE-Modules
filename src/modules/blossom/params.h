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
  static auto constexpr unit = "x";

  static inline auto ratio(float rotation, bool quantize) -> float {
    auto const ratio = range.scale(rotation);
    return quantize ? std::round(ratio) : ratio;
  }

  static auto constexpr rotation(float ratio) -> float {
    return range.normalize(ratio);
  }

  struct KnobMapper {
    auto to_value(float ratio) const -> float { return rotation(ratio); }

    auto to_display_value(float rotation) const -> float {
      return ratio(rotation, quantize_);
    }

    void quantize(bool q) { quantize_ = q; }

  private:
    bool quantize_{false};
  };

private:
  static constexpr auto range = Range{1.F, 17.F};
};

struct BounceRatioModes {
  enum Index { Quantized, Free };
  using ValueType = Index;

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{"Quantized", "Free"};
    return labels;
  }
};

class SpinSpeed {
  static auto constexpr range = Range{-10.F, 10.F};
  static auto constexpr taper = sigmoid::s_taper_with_curvature(-0.8F);

public:
  static auto constexpr unit = " Hz";

  static auto constexpr rotation(float hertz) -> float {
    return taper.invert(range.normalize(hertz));
  }

  static auto constexpr hertz(float rotation) -> float {
    return range.scale(taper.apply(rotation));
  }

  struct KnobMapper {
    auto to_display_value(float rotation) const -> float {
      return hertz(rotation);
    }

    auto to_value(float hertz) const -> float { return rotation(hertz); }
  };
};

} // namespace blossom
} // namespace dhe
