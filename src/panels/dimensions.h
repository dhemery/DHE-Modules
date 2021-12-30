#pragma once

namespace dhe {

static auto constexpr mm_per_in = 25.4F;
static auto constexpr px_per_in = 75.F;
static auto constexpr px_per_mm = px_per_in / mm_per_in;

static inline auto constexpr mm2px(float mm) -> float { return mm * px_per_mm; }

static auto constexpr button_diameter = 6.F;
static auto constexpr button_radius = button_diameter / 2.F;

static auto constexpr light_radius = 1.088F;
static auto constexpr light_diameter = light_radius * 2.F;

static auto constexpr port_diameter = 8.4F;
static auto constexpr port_radius = port_diameter / 2.F;

static auto constexpr small_knob_diameter = 8.4F;
static auto constexpr small_knob_radius = small_knob_diameter / 2.F;

static auto constexpr tiny_knob_diameter = 7.F;
static auto constexpr tiny_knob_radius = tiny_knob_diameter / 2.F;

static auto constexpr proxima_nova_ascent_ratio = 2.F / 3.F;
static auto constexpr small_font_size = 7.0F / px_per_mm;
static auto constexpr small_font_ascent =
    small_font_size * proxima_nova_ascent_ratio;

} // namespace dhe
