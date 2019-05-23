#pragma once

#include <array>
#include <functional>

#include <engine/Module.hpp>

#include "util/range.h"

namespace dhe {
namespace duration {

static Range constexpr shortRange{0.001f, 1.f};
static Range constexpr mediumRange{0.01f, 10.f};
static Range constexpr longRange{0.1f, 100.f};

extern std::array<Range const *, 3> const ranges;

/**
 * Returns the range for the given range switch position.
 * @param switchPosition the position of the range switch
 */
auto range(int switchPosition) -> Range const *;

/**
 * Creates a function that yields the duration (in seconds) selected by a
 * cv-modulated knob and a duration range switch.
 * @param knobId the ID of the duration knob param
 * @param cvId the ID of the control voltage input
 * @param switchId the ID of the duration range switch
 */
auto withSelectableRange(rack::engine::Module *module, int knobId, int cvId,
                         int switchId) -> std::function<float()>;

/**
 * Creates a function that yields the duration (in seconds) selected by a knob
 * from a given range.
 * @param knobId the ID of the duration knob param
 * @param range the range from which to select the duration
 */
auto withFixedRange(rack::engine::Module *module, int knobId,
                    Range const &range) -> std::function<float()>;

/**
 * Creates a function that  maps a rotation to a "duration-tapered" value. The
 * duration-tapered value is useful for selecting a duration from one of the
 * duration ranges.
 */
auto rotationToDurationTaper() -> std::function<float(float)>;

/**
 * Creates a function that maps a duration-tapered value [0.0–1.0] to a
 * rotation.
 */
auto durationTaperToRotation() -> std::function<float(float)>;

} // namespace duration
} // namespace dhe
