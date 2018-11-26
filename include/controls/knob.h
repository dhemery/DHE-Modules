#include <utility>

#pragma once

#include <utility>

#include <engine.hpp>

#include "util/range.h"

namespace DHE {

class Knob {
  const rack::Module *module;
  const rack::Param *knob_param;
  const std::function<float(float)> mapped{&identity};

public:
  Knob(const rack::Module *module, int knob_index)
      : module{module}, knob_param{&module->params[knob_index]} {}

  auto operator()() const -> float { return mapped(knob_param->value); }

  auto modulate_by(int cv_index) -> Knob {
    return map(modulated_by(module, cv_index));
  }

  auto modulate_by(int cv_index, int av_index) -> Knob {
    return map(modulated_by(module, cv_index, av_index));
  }

  template <typename Mapper> auto map(Mapper mapper) -> Knob {
    return Knob{*this, mapper};
  }

  const Knob scale_to(const Range range) {
    const auto to_range = [range](float in) -> float {
      return range.scale(in);
    };
    return map(to_range);
  }

  template <typename Scaler> const Knob scale_to(Scaler scaler) {
    const auto to_selected_range = [scaler](float in) -> float {
      return scaler().scale(in);
    };
    return map(to_selected_range);
  }

private:
  static auto modulate(float rotation, float cv = 0.f, float av = 1.f)
      -> float {
    static constexpr auto av_range = Range{-1.f, 1.f};
    static constexpr auto cv_to_offset = 0.1f;

    auto offset = cv * cv_to_offset;
    auto multipler = av_range.scale(av);
    return rotation + multipler * offset;
  }

  static auto modulated_by(const rack::Module *module, int cv_index)
      -> const std::function<float(float)> {
    const auto *cv_input = &module->inputs[cv_index];
    return [cv_input](float rotation) -> float {
      return modulate(rotation, cv_input->value);
    };
  }

  static auto modulated_by(const rack::Module *module, int cv_index,
                           int av_index) -> const std::function<float(float)> {
    const auto *cv_input = &module->inputs[cv_index];
    const auto *av_param = &module->params[av_index];
    return [cv_input, av_param](float rotation) -> float {
      return modulate(rotation, cv_input->value, av_param->value);
    };
  }

  static auto identity(float in) -> float { return in; }

  template <typename Mapper>
  Knob(const Knob source, Mapper mapper)
      : module{source.module}, knob_param{source.knob_param},
        mapped{[source, mapper](float in) -> float {
          return mapper(source.mapped(in));
        }} {};
};
} // namespace DHE
