#pragma once

#include "controls/switch.h"
#include "range.h"

namespace DHE {
namespace Sigmoid {

/**
 * Applies an inverse sigmoid function to the input.
 * <p>
 * The curvature determines the shape and intensity of the transfer function.
 * A positive curvature applies an inverted S-shaped transfer function.
 * A curvature of 0 applies a linear transfer function.
 * A negative curvature applies an S-shaped transfer function.
 * <p>
 * Before the function is applied:
 * <ul>
 * <li>The input is clamped to the range [-1.0, 1.0].</li>
 * <li>The curvature is clamped to the range [0.0001, 0.9999].</li>
 * </ul>
 * @param input the input to the inverse sigmoid function
 * @param curvature the intensity and direction of the curvature
 * @return the sigmoid function result
 */
auto inverse(float input, float curvature) -> float;

/**
 * Applies a sigmoid function to the input.
 * <p>
 * The curvature determines the shape and intensity of the transfer function.
 * A positive curvature applies an S-shaped transfer function.
 * A curvature of 0 applies a linear transfer function.
 * A negative curvature applies an inverted S-shaped transfer function.
 * <p>
 * Before the function is applied:
 * <ul>
 * <li>The input is clamped to the range [-1.0, 1.0].</li>
 * <li>The curvature is clamped to the range [0.0001, 0.9999].</li>
 * </ul>
 * @param input the input to the sigmoid function
 * @param curvature the intensity and direction of the curvature
 * @return the sigmoid function result
 */
inline auto curve(float input, float curvature) -> float {
  return inverse(input, -curvature);
}

/**
 * Applies a gentle S-shaped transfer function to map an input in the range
 * [0.0, 1.0] to an output in the range [-1.0, 1.0]. The transfer function makes
 * the output more sensitive to changes in inputs near 0.5 and less sensitive to
 * changes near 0.0 and 1.0.
 * <p>
 * This function is intended to translate DHE-Modules CURVE knob rotation to a
 * curvature value suitable to pass to the curve(), inverse(), j_taper(), and
 * s_taper() functions.
 *
 * @param input the value to map to a curvature
 * @return the curvature
 */
auto curvature(float input) -> float;

/**
 * Applies a J-shaped transfer function to the input.
 * <p>
 * The curvature determines the shape and intensity of the taper.
 * A positive curvature applies a J-taper.
 * A curvature of 0 applies a linear taper.
 * A negative curvature applies an inverted J-taper.
 * <p>
 * Before the function is applied:
 * <ul>
 * <li>The input is clamped to the range [0.0, 1.0].</li>
 * <li>The curvature is clamped to the range [0.0001, 0.9999].</li>
 * </ul>
 * @param input the input to the taper function
 * @param curvature the intensity and direction of the taper
 * @return the taper function result
 */
auto j_taper(float input, float curvature) -> float;

/**
 * Applies an S-shaped transfer function to the input.
 * <p>
 * The curvature determines the shape and intensity of the taper.
 * A positive curvature applies an S-taper.
 * A curvature of 0 applies a linear taper.
 * A negative curvature applies an inverted S-taper.
 * <p>
 * Before the function is applied:
 * <ul>
 * <li>The input is clamped to the range [0.0, 1.0].</li>
 * <li>The curvature is clamped to the range [0.0001, 0.9999].</li>
 * </ul>
 * @param input the input to the taper function
 * @param curvature the intensity and direction of the taper
 * @return the taper function result
 */
auto s_taper(float input, float curvature) -> float;

auto shape(float input, float curvature, bool is_s) -> float;

class Shaper {
  Knob curve_knob;
  Switch<bool> is_s;

public:
  Shaper(Knob curve_knob, Switch<bool> is_s)
      : curve_knob{std::move(curve_knob)}, is_s{std::move(is_s)} {}

  auto operator()(float input) const -> float {
    return shape(input, curve_knob(), is_s());
  }
};

} // namespace Sigmoid
} // namespace DHE
