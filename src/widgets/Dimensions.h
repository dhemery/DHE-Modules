#pragma once

#include "PanelAssets.h"

#include <string>
#include <widget/Widget.hpp>

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

static inline auto mm2px(float xmm, float ymm) -> rack::math::Vec { return {mm2px(xmm), mm2px(ymm)}; }
static inline auto mm2px(rack::math::Vec const &mm) -> rack::math::Vec { return mm2px(mm.x, mm.y); }

static inline void positionCentered(rack::widget::Widget *widget, float xmm, float ymm) {
  auto const &center = mm2px(xmm, ymm);
  auto const &offset = widget->box.size.div(2);
  widget->setPosition(center.minus(offset));
}

auto constexpr moduleHeight = 128.5F; // mm
auto constexpr portRadius = 4.2F;
auto constexpr buttonRadius = 3.F;
auto constexpr lightRadius = 1.088F;
auto constexpr lightDiameter = lightRadius * 2.F;
auto constexpr buttonPortDistance = 7.891F;

} // namespace dhe
