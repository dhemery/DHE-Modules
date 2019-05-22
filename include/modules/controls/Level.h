#pragma once

#include <array>

#include "util/range.h"

namespace dhe {
namespace level {

static Range constexpr unipolarRange{0.f, 10.f};
static Range constexpr bipolarRange{-5.f, 5.f};

extern std::array<Range const *, 2> const ranges;

/**
 * Returns the range for the given range switch position.
 * @param switchPosition the position of the range switch
 */
auto range(float switchPosition) -> Range const *;

auto withSelectableRange(rack::engine::Module *module, int knobId, int cvId,
                         int switchId) -> std::function<float()>;

auto withFixedRange(rack::engine::Module *module, int knobId,
                    Range const &range) -> std::function<float()>;
} // namespace level
} // namespace dhe
