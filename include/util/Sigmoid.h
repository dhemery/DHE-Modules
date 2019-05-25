#pragma once
#include "Range.h"

#include <array>
#include <cmath>

namespace dhe {
namespace sigmoid {

  static constexpr auto sigmoidRange = Range{-1.f, 1.f};
  static constexpr auto proportionRange = Range{0.f, 1.f};

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
  static inline auto inverse(float input, float curvature) -> float {
    static constexpr auto precision = 1e-4f;
    static constexpr auto maxCurvature = 1.0f - precision;
    static constexpr auto curvatureRange = Range{-maxCurvature, maxCurvature};

    curvature = curvatureRange.clamp(curvature);
    input = sigmoidRange.clamp(input);

    return (input - input * curvature) / (curvature - std::abs(input) * 2.0f * curvature + 1.0f);
  }

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
  static inline auto curve(float input, float curvature) -> float { return inverse(input, -curvature); }

  class Shape {
  public:
    virtual auto taper(float input, float curvature) const -> float = 0;
  };

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
   */
  class JShape : public Shape {
  public:
    /**
     * Applies a J-shaped transfer function to the input.
     * @param input the input value
     * @param curvature the intensity of the taper
     */
    auto taper(float input, float curvature) const -> float override {
      return inverse(proportionRange.clamp(input), curvature);
    }
  };
  static constexpr auto jShape = JShape{};

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
   */
  class SShape : public Shape {
  public:
    /**
     * Applies an S-shaped transfer function to the input.
     * @param input the input value
     * @param curvature the intensity of the taper
     */
    auto taper(float input, float curvature) const -> float override {
      const auto scaled = sigmoidRange.scale(input);
      const auto tapered = curve(scaled, curvature);
      return sigmoidRange.normalize(tapered);
    }
  };

  static constexpr auto sShape = SShape{};

  extern std::array<Shape const *, 2> const shapes;

  /**
   * Applies a gentle S-shaped transfer function to map an input in the range
   * [0.0, 1.0] to an output in the range [-1.0, 1.0]. The transfer function
   * makes the output more sensitive to changes in inputs near 0.5 and less
   * sensitive to changes near 0.0 and 1.0. <p> This function is intended to
   * translate DHE-Modules CURVE knob rotation to a curvature value suitable to
   * pass to the curve(), inverse(), j_taper(), and s_taper() functions.
   *
   * @param input the value to map to a curvature
   * @return the curvature
   */
  static inline auto curvature(float input) -> float {
    // This curvature creates a gentle S curve, increasing sensitivity in the
    // middle of the input range and decreasing sensitivity toward the extremes.
    static constexpr auto gentleS = 0.65f;
    auto scaled = sigmoidRange.scale(input);
    return curve(scaled, gentleS);
  }

  static inline auto jTaper(float input, float curvature) -> float {
    return inverse(proportionRange.clamp(input), curvature);
  }

  static inline auto sTaper(float input, float curvature) -> float {
    const auto scaled = sigmoidRange.scale(input);
    const auto tapered = curve(scaled, curvature);
    return sigmoidRange.normalize(tapered);
  }

  static inline auto taper(float input, float curvature, bool isS) -> float {
    return isS ? sTaper(input, curvature) : jTaper(input, curvature);
  }

} // namespace sigmoid
} // namespace dhe
