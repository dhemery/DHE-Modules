#pragma once

#include <functional>

#include "util/range.h"
#include <engine/Module.hpp>

namespace dhe {

namespace control {

namespace knob {
/**
 * Creates a function that returns the rotation of a knob.
 * @param knobId the ID of the knob param
 */
auto rotation(rack::engine::Module *module, int knobId)
    -> std::function<float()>;

/**
 * Creates a function that returns the modulated rotation of a knob. The amount
 * of modulation is determined by the voltage of a CV input.
 * @param knobId the ID of the knob param
 * @param cvId the ID of the CV input
 */
auto rotation(rack::engine::Module *module, int knobId, int cvId)
    -> std::function<float()>;

/**
 * Creates a function that returns the modulated rotation of a knob. The amount
 * of modulation is determined by the voltage of a CV input, multiplied by the
 * value of an attenuverter.
 * @param knobId the ID of the knob param
 * @param cvId the ID of the CV input
 * @param avId the ID of the attenuverter param
 */
auto rotation(rack::engine::Module *module, int knobId, int cvId, int avId)
    -> std::function<float()>;

/**
 * Creates a function that scales a rotation.
 */
auto scaled(std::function<float()> const &rotation,
            std::function<float(float)> const &scale) -> std::function<float()>;

/**
 * Creates a function that scales a tapered rotation
 */
auto scaled(std::function<float()> const &rotation,
            std::function<float(float)> const &taper,
            std::function<float(float)> const &scale) -> std::function<float()>;

} // namespace knob

namespace scale {
auto toRange(std::function<Range const *()> range)
    -> std::function<float(float)>;

auto toRange(Range const &) -> std::function<float(float)>;

} // namespace scale

namespace range {
template <int N>
auto selection(rack::engine::Module *module, int switchId,
               std::array<dhe::Range const *, N> const &ranges)
    -> std::function<dhe::Range const *()> {
  auto switchParam = &module->params[switchId];
  return [switchParam, ranges]() -> dhe::Range {
    auto const selection = static_cast<int>(switchParam->getValue());
    return ranges[selection];
  };
}
} // namespace range
} // namespace control
} // namespace dhe
