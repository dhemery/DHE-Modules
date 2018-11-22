#pragma once
#include <functional>
#include <vector>

#include "range.hpp"
#include "sigmoid.hpp"

namespace DHE {

namespace Duration {
auto constexpr short_range{Range{0.001f, 1.f}};
auto constexpr medium_range{Range{0.01f, 10.f}};
auto constexpr long_range{Range{0.1f, 100.f}};

/**
 * Creates a function that applies the given proportion and selector suppliers
 * to select a duration. <p> The selector supplier must supply an integer in the
 * range 0–2. The created function applies the selector to select a duration
 * range as follows: <ul> <li>0 selects a short duration range: 0.001–1.0
 * seconds.</li> <li>1 selects a medium duration range: 0.01–10.0 seconds.</li>
 * <li>2 selects a long duration range: 0.1–100.0 seconds.</li>
 * </ul>
 * <p>
 * The proportion supplier must supply a float in the range 0.0–1.0. The created
 * function applies a J taper to the proportion to select a value within the
 * duration range as follows:
 * <ul>
 * <li>0.0 selects the lower bound of the range.</li>
 * <li>0.5 selects a value approximately 1/10 the upper bound of the
 * range.</li>
 * <li>1.0 selects the upper bound of the range.</li>
 * </ul>
 *
 * @param selector selects the duration range
 * @param proportion selects the duration within the duration range
 * @return a function that yields the selected duration
 */
template <typename Proportion, typename Selector>
auto of(Proportion const &proportion, Selector const &selector)
    -> std::function<float()> {
  static auto const ranges{std::vector<Range const>{short_range, medium_range, long_range}};

  // Shapes the J taper to map an input of 0.5 to an output of ~0.1. Thus a
  // proportion of 0.5 will yield a duration of ~1/10 of the range's maximum.
  auto constexpr knob_curvature{0.8f};

  return [&proportion, &selector]() -> float {
    // Apply the J taper to the proportion.
    auto j_tapered_proportion{sigmoid(proportion(), knob_curvature)};

    // Scale the tapered proportion to the desired range.
    return ranges[selector()].scale(j_tapered_proportion);
  };
}

/**
 * Creates a function that applies the given proportion supplier to select a
 * duration in the range 0.01–10.0 seconds.
 * <p>
 * <p>
 * The proportion supplier must supply a float in the range 0.0–1.0. The created
 * function applies a J taper to the proportion to select a value within the
 * duration range as follows:
 * <ul>
 * <li>0.0 selects the lower bound of the range.</li>
 * <li>0.5 selects a value approximately 1/10 the upper bound of the
 * range.</li>
 * <li>1.0 selects the upper bound of the range.</li>
 *
 * @param proportion selects the duration
 * @return a function that yields the selected duration
 */
template <typename Proportion>
auto of(Proportion const &proportion) -> std::function<float()> {
  static auto const select_medium_range{[]() -> int { return 1; }};

  return of(proportion, select_medium_range);
}

} // namespace Duration
} // namespace DHE
