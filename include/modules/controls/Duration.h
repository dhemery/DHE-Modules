#pragma once

#include <array>
#include <functional>

#include <engine/Module.hpp>
#include "Controls.h"

#include "util/range.h"

namespace DHE {
namespace Duration {

static Range constexpr shortRange{0.001f, 1.f};
static Range constexpr mediumRange{0.01f, 10.f};
static Range constexpr longRange{0.1f, 100.f};

/**
 * Each duration range is of the form [n, 1000n]. Given ranges of that form,
 * this curvature tapers the rotation so a knob positioned dead center yields
 * a duration equal to 1/10 of the range's upper bound (to within 7 decimal
 * places).
 */
extern float const knobTaperCurvature;

extern std::array<Range const *, 3> const ranges;

/**
 * Returns the range for the given range switch position.
 * @param switchPosition the position of the range switch
 */
auto range(int switchPosition) -> Range const *;

auto withCvAndSwitch(rack::engine::Module *module, int knobId, int cvId,
                     int switchId) -> std::function<float()>;

auto withRange(rack::engine::Module *module, int knobId, Range const &range) -> std::function<float()>;
} // namespace Duration
} // namespace DHE
