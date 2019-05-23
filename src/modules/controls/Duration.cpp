#include <engine/Module.hpp>

#include "modules/controls/Controls.h"
#include "modules/controls/Duration.h"
#include "util/sigmoid.h"

namespace dhe {

namespace duration {

/**
 * Each duration range is of the form [n, 1000n]. Given ranges of that form,
 * this curvature tapers the rotation so a knob positioned dead center
 * yields a duration equal to 1/10 of the range's upper bound (to within 7
 * decimal places).
 */
float const knobTaperCurvature = 0.8018017;

const std::array<Range const *, 3> ranges{&shortRange, &mediumRange,
                                          &longRange};

auto range(float switchPosition) -> Range const * {
  return ranges[static_cast<int>(switchPosition)];
}

auto rotationToDurationTaper() -> std::function<float(float)> {
  return [](float rotation) -> float {
    return sigmoid::j_taper(rotation, knobTaperCurvature);
  };
}

auto durationTaperToRotation() -> std::function<float(float)> {
  return [](float rotation) -> float {
    return sigmoid::j_taper(rotation, -knobTaperCurvature);
  };
}

auto withFixedRange(rack::engine::Module *module, int knobId,
                    Range const &range) -> std::function<float()> {
  using namespace control;
  auto const durationRotation = knob::rotation(module, knobId);
  auto const durationKnobTaper = duration::rotationToDurationTaper();
  auto const toDurationRange = scale::toRange(range);
  return knob::scaled(durationRotation, durationKnobTaper, toDurationRange);
}

auto withSelectableRange(rack::engine::Module *module, int knobId, int cvId,
                         int switchId) -> std::function<float()> {
  using namespace control;
  auto const durationRotation = knob::rotation(module, knobId, cvId);
  auto const durationKnobTaper = duration::rotationToDurationTaper();
  auto const selectedDurationRange =
      range::selection<3>(module, switchId, duration::ranges);
  auto const toDurationRange = scale::toRange(selectedDurationRange);
  return knob::scaled(durationRotation, durationKnobTaper, toDurationRange);
}
} // namespace duration
} // namespace dhe
