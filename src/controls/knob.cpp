#include <utility>

#include <controls/knob.h>
#include <engine.hpp>

namespace DHE {


auto Knob::map(const std::function<float(float)> &wrapper) const -> Knob {
  auto old = this->function;
  auto wrapped = [old, wrapper](float input) -> float { return wrapper(old(input)); };
  return Knob{module, knob, cv, av, wrapped};
}

auto Knob::scale_to(Range range) const -> Knob {
  const auto to_range = [range](float in) -> float { return range.scale(in); };
  return map(to_range);
}

auto Knob::scale_to(const std::function<Range()> &range_supplier) const
    -> Knob {
  const auto to_selected_range = [range_supplier](float in) -> float {
    return range_supplier().scale(in);
  };
  return map(to_selected_range);
}

auto Knob::default_cv() -> const rack::Input * {
  static auto cv_input = rack::Input{};
  cv_input.value = 0.f;
  return &cv_input;
}

auto Knob::default_av() -> const rack::Param * {
  static auto av_param = rack::Param{};
  av_param.value = 1.f;
  return &av_param;
}

auto Knob::identity(float input) -> float {
  return input;
}
} // namespace DHE
