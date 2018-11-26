#include <utility>

#pragma once

#include <utility>

#include <engine.hpp>

#include "util/range.h"

namespace DHE {

class Knob {
public:
  Knob(const rack::Module *module, int knob_index)
      : module{module}, knob_param{&module->params[knob_index]} {}

  auto operator()() const -> float;

  auto modulate_by(int cv_index) const -> Knob;

  auto modulate_by(int cv_index, int av_index) const -> Knob;

  template <typename Mapper> auto map(Mapper mapper) const -> Knob {
    return Knob{*this, mapper};
  }

  auto scale_to(Range range) const -> Knob;

  template <typename Scaler> auto scale_to(Scaler scaler) const -> Knob {
    const auto to_selected_range = [scaler](float in) -> float {
      return scaler().scale(in);
    };
    return map(to_selected_range);
  }

private:
  static auto identity(float in) -> float;

  template <typename Mapper>
  Knob(const Knob source, Mapper mapper)
      : module{source.module}, knob_param{source.knob_param},
        mapped{[source, mapper](float in) -> float {
          return mapper(source.mapped(in));
        }} {};

  const rack::Module *module;
  const rack::Param *knob_param;
  const std::function<float(float)> mapped{&identity};
};
} // namespace DHE
