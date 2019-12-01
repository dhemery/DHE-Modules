#pragma once

#include <helpers.hpp>

namespace dhe {

template <typename Param, typename Panel, typename Module>
auto installParam(Panel *panel, Module *module, float x, float y, int index) -> Param * {
  auto *widget = rack::createParamCentered<Param>(mm2px(x, y), module, index);
  widget->shadow->opacity = 0.F;
  panel->addParam(widget);
  return widget;
}

} // namespace dhe
