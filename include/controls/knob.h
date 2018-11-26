#include <utility>

#include <utility>

#pragma once

#include <utility>

#include <engine.hpp>

#include "util/range.h"

namespace DHE {

class Knob {
public:
  Knob(const rack::Module *module, int knob_index);

  auto operator()() const -> float;

  auto modulate_by(int cv_index) const -> Knob;

  auto modulate_by(int cv_index, int av_index) const -> Knob;

  auto map(const std::function<float(float)> &mapper) const -> Knob;

  auto scale_to(Range range) const -> Knob;

  auto scale_to(const std::function<Range()> &range_supplier) const -> Knob;

private:
  Knob(const rack::Module *module, const rack::Param *knob_param,
       const std::function<float(float)> &mapper);

  const rack::Module *module;
  const rack::Param *knob_param;
  const std::function<float(float)> mapper;
};
} // namespace DHE
