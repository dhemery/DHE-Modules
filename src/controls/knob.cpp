#include <utility>

#include <controls/knob.h>
#include <engine.hpp>

namespace DHE {

inline auto modulate(float rotation, float cv = 0.f, float av = 1.f) -> float {
  static constexpr auto av_range = Range{-1.f, 1.f};
  static constexpr auto cv_to_offset = 0.1f;

  auto offset = cv * cv_to_offset;
  auto multipler = av_range.scale(av);
  return rotation + multipler * offset;
}

inline auto modulated_by(const rack::Module *module, int cv_index)
    -> const std::function<float(float)> {
  const auto *cv_input = &module->inputs[cv_index];
  return [cv_input](float rotation) -> float {
    return modulate(rotation, cv_input->value);
  };
}

inline auto modulated_by(const rack::Module *module, int cv_index, int av_index)
    -> const std::function<float(float)> {
  const auto *cv_input = &module->inputs[cv_index];
  const auto *av_param = &module->params[av_index];
  return [cv_input, av_param](float rotation) -> float {
    return modulate(rotation, cv_input->value, av_param->value);
  };
}

inline auto value_of(const rack::Param *param) -> std::function<float()> {
  return [param] { return param->value; };
}

Knob::Knob(const rack::Module *module, int knob_index)
    : Knob{module, value_of(&module->params[knob_index])} {}

Knob::Knob(const rack::Module *module, const std::function<float()> &supplier)
    : module{module}, supplier{supplier} {}

auto Knob::map(const std::function<float(float)> &mapper) const -> Knob {
  auto supplier = this->supplier;
  auto new_supplier = [supplier, mapper] { return mapper(supplier()); };
  return Knob{module, new_supplier};
}

auto Knob::modulate_by(int cv_index) const -> Knob {
  return map(modulated_by(module, cv_index));
}

auto Knob::modulate_by(int cv_index, int av_index) const -> Knob {
  return map(modulated_by(module, cv_index, av_index));
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

} // namespace DHE
