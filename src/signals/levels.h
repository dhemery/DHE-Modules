#pragma once

#include "components/range.h"
#include "controls/switches.h"

#include "rack.hpp"

#include <functional>
#include <string>
#include <vector>

namespace dhe {
template <typename TRanges>
struct RangeSwitchQuantity : public rack::engine::SwitchQuantity {
  using TAction = std::function<void(Range)>;

  void setValue(float f) override {
    rack::engine::SwitchQuantity::setValue(f);
    auto const selection = static_cast<int>(f);
    action_(TRanges::ranges()[selection]);
  }

  void set_action(TAction const &action) { action_ = action; }

private:
  std::function<void(Range)> action_ = [](Range) {};
};

struct RangedParamQuantity : public rack::engine::ParamQuantity {
  void set_range(Range r) {
    auto const old_range = Range{minValue, maxValue};
    auto const rotation = old_range.normalize(getValue());
    auto const default_rotation = old_range.normalize(defaultValue);
    minValue = r.lower_bound();
    maxValue = r.upper_bound();
    defaultValue = r.scale(default_rotation);
    setValue(r.scale(rotation));
  }
};

struct Levels {
  enum { Bipolar, Unipolar };

  static auto constexpr unit = " V";

  static inline auto ranges() -> std::vector<Range> const & {
    static auto const ranges =
        std::vector<Range>{bipolar_signal_range, unipolar_signal_range};
    return ranges;
  }

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{"±5 V", "0–10 V"};
    return labels;
  }
};

struct LevelSwitch {
  using TQuantity = RangeSwitchQuantity<Levels>;

  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name,
                            int selection = Levels::Unipolar) -> TQuantity * {
    auto const max_value = static_cast<float>(Levels::ranges().size() - 1);
    auto const default_value = static_cast<float>(selection);
    auto *pq = module->configSwitch<TQuantity>(
        id, 0.F, max_value, default_value, name, Levels::labels());
    return pq;
  }
};

struct LevelKnob {
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, float initial_value = 5.F,
                            int initial_range = Levels::Unipolar)
      -> RangedParamQuantity * {
    auto const range = Levels::ranges()[initial_range];
    auto *pq = module->configParam<RangedParamQuantity>(
        id, range.lower_bound(), range.upper_bound(), initial_value, name,
        Levels::unit);
    return pq;
  }
};
} // namespace dhe
