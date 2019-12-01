#pragma once

#include "Dimensions.h"

#include <helpers.hpp>
#include <math.hpp>

namespace dhe {

template <typename Param, typename Panel, typename Module>
auto installParam(Panel *panel, Module *module, float x, float y, int index) -> Param * {
  auto *widget = rack::createParamCentered<Param>(mm2px(x, y), module, index);
  widget->shadow->opacity = 0.F;
  panel->addParam(widget);
  return widget;
}

template <typename Light, typename Panel, typename Module>
auto installLight(Panel *panel, Module *module, float x, float y, int index) -> Light * {
  auto *light = rack::createLightCentered<Light>(mm2px(x, y), module, index);
  panel->addChild(light);
  return light;
}

template <typename Widget, typename Panel> auto installWidget(Panel *panel, rack::math::Vec pos) -> Widget * {
  auto *widget = rack::createWidgetCentered<Widget>(mm2px(pos));
  panel->addChild(widget);
  return widget;
}

} // namespace dhe
