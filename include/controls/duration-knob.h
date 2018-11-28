#pragma once

#include "knob.h"
#include "util/range.h"
#include "util/sigmoid.h"

namespace DHE {

class DurationKnob {
public:
  DurationKnob(const rack::Module *module, int knob_index)
      : knob{&module->params[knob_index].value}, cv{&constant_zero},
        range_switch{&constant_one} {}
  DurationKnob(const rack::Module *module, int knob_index, int cv_index,
               int range_switch_index)
      : knob{&module->params[knob_index].value},
        cv{&module->inputs[cv_index].value},
        range_switch{&module->params[range_switch_index].value} {}

  inline auto operator()() const -> float {
    auto rotation = modulated(*knob, *cv);
    auto tapered = Sigmoid::j_taper(rotation, 0.8f);
    return range().scale(tapered);
  }

private:
  auto range() const -> const Range & {
    static constexpr auto short_range = Range{0.001f, 1.f};
    static constexpr auto medium_range = Range{0.01f, 10.f};
    static constexpr auto long_range = Range{0.1f, 100.f};

    auto position = *range_switch;
    if (position > 1.5f)
      return long_range;
    if (position < 0.5f)
      return short_range;
    return medium_range;
  }

  const float *knob;
  const float *cv;
  const float *range_switch;
};

} // namespace DHE
