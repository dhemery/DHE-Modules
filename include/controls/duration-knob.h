#pragma once

#include "knob.h"
#include "util/range.h"
#include "util/sigmoid.h"

namespace DHE {

class DurationKnob : Knob {
public:
  DurationKnob(const rack::Module *module, int knob_index)
      : Knob{module, knob_index} {}

  DurationKnob(const rack::Module *module, int knob_index, int cv_index,
               int range_switch_index)
      : Knob{module, knob_index, cv_index},
        range_switch{&module->params[range_switch_index]} {}

  auto operator()() const -> float override {
    auto rotation = DHE::Knob::operator()();
    auto tapered = Sigmoid::j_taper(rotation, 0.8f);
    return range().scale(tapered);
  }

private:
  static auto default_range_switch() -> const rack::Param * {
    static auto range_switch = rack::Param{};
    range_switch.value = 1.f;
    return &range_switch;
  }

  auto range() const -> const Range & {
    static constexpr auto short_range = Range{0.001f, 1.f};
    static constexpr auto medium_range = Range{0.01f, 10.f};
    static constexpr auto long_range = Range{0.1f, 100.f};

    auto position = range_switch->value;
    if (position > 1.5f)
      return long_range;
    if (position < 0.5f)
      return short_range;
    return medium_range;
  }

  const rack::Param *range_switch{default_range_switch()};
};
} // namespace DHE
