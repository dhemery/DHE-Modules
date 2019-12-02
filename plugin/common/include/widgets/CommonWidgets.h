#pragma once

#include "Dimensions.h"

#include <engine/Module.hpp>
#include <helpers.hpp>
#include <math.hpp>
#include <widget/Widget.hpp>

namespace dhe {

static inline void positionCentered(rack::widget::Widget *widget, float xmm, float ymm) {
  widget->box.pos = mm2px(xmm, ymm).minus(widget->box.size.div(2));
}

template <typename Light, typename Panel>
auto installLight(Panel *panel, rack::engine::Module *module, float x, float y, int index) -> Light * {
  auto *light = rack::createLightCentered<Light>(mm2px(x, y), module, index);
  panel->addChild(light);
  return light;
}
} // namespace dhe
