#pragma once

#include "panel-assets.h"

#include "rack.hpp"

#include <string>

namespace dhe {
static auto constexpr px_per_hp = 15.F;
static auto constexpr mm_per_in = 25.4F;
static auto constexpr px_per_in = 75.F;
static auto constexpr mm_per_hp = 5.08F;
static auto constexpr px_per_mm = px_per_in / mm_per_in;

static inline auto constexpr hp2px(float hp) -> float { return hp * px_per_hp; }
static inline auto constexpr hp2mm(float hp) -> float { return hp * mm_per_hp; }
static inline auto constexpr mm2px(float mm) -> float { return mm * px_per_mm; }
static inline auto constexpr mm2hp(float mm) -> float { return mm / mm_per_hp; }
static inline auto constexpr i2mm(float mm) -> float { return mm / mm_per_in; }

static inline auto mm2px(float xmm, float ymm) -> rack::math::Vec {
  return {mm2px(xmm), mm2px(ymm)};
}
static inline auto mm2px(rack::math::Vec const &mm) -> rack::math::Vec {
  return mm2px(mm.x, mm.y);
}

static inline void position_centered(rack::widget::Widget *widget, float xmm,
                                     float ymm) {
  auto const &center = mm2px(xmm, ymm);
  auto const &offset = widget->box.size.div(2);
  widget->setPosition(center.minus(offset));
}

static auto constexpr module_height = 128.5F; // mm
static auto constexpr port_radius = 4.2F;
static auto constexpr port_diameter = port_radius * 2.F;
static auto constexpr button_radius = 3.F;
static auto constexpr button_diameter = button_radius * 2.F;
static auto constexpr light_radius = 1.088F;
static auto constexpr light_diameter = light_radius * 2.F;
static auto constexpr button_port_distance = 7.891F;
static auto constexpr padding = 1.F;
static auto constexpr small_label_size = 7.0F / px_per_mm;
static auto constexpr small_knob_radius = 4.2F;
static auto constexpr small_knob_diameter = small_knob_radius * 2.F;

} // namespace dhe
