#pragma once

#include "PanelAssets.h"

#include <app/SvgSwitch.hpp>
#include <string>

namespace dhe {
auto constexpr pxPerHp = 15.F;
auto constexpr mmPerIn = 25.4F;
auto constexpr pxPerIn = 75.F;
auto constexpr mmPerHp = 5.08F;
auto constexpr pxPerMm = pxPerIn / mmPerIn;

auto constexpr hp2px(float hp) -> float { return hp * pxPerHp; }
auto constexpr hp2mm(float hp) -> float { return hp * mmPerHp; }
auto constexpr mm2px(float mm) -> float { return mm * pxPerMm; };
auto constexpr mm2hp(float mm) -> float { return mm / mmPerHp; };

static inline auto mm2px(float x, float y) -> rack::math::Vec { return {mm2px(x), mm2px(y)}; }
static inline auto mm2px(rack::math::Vec const &mm) -> rack::math::Vec { return mm2px(mm.x, mm.y); }

auto constexpr moduleHeight = 128.5F; // mm
auto constexpr portRadius = 4.2F;
auto constexpr buttonRadius = 3.F;
auto constexpr lightRadius = 1.088F;
auto constexpr lightDiameter = lightRadius * 2.F;
auto constexpr buttonPortDistance = 7.891F;

} // namespace dhe
