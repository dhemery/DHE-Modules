#pragma once

#include "Dimensions.h"

#include <app/SvgPort.hpp>
#include <engine/Module.hpp>
#include <helpers.hpp>
#include <math.hpp>

namespace dhe {

template <typename Widget, typename Panel> auto installWidget(Panel *panel, rack::math::Vec pos) -> Widget * {
  auto *widget = rack::createWidgetCentered<Widget>(mm2px(pos));
  panel->addChild(widget);
  return widget;
}

template <typename Param, typename Panel>
auto installParam(Panel *panel, rack::engine::Module *module, float x, float y, int index) -> Param * {
  auto *widget = rack::createParamCentered<Param>(mm2px(x, y), module, index);
  widget->shadow->opacity = 0.F;
  panel->addParam(widget);
  return widget;
}

template <template <typename> class Param, typename Panel>
static auto install(Panel *panel, rack::engine::Module *module, float x, float y, int index) -> Param<Panel> * {
  return installParam<Param<Panel>>(panel, module, x, y, index);
}

template <template <typename, int> class Param, int N, typename Panel>
static auto install(Panel *panel, rack::engine::Module *module, float x, float y, int index) -> Param<Panel, N> * {
  return installParam<Param<Panel, N>>(panel, module, x, y, index);
}

template <typename Light, typename Panel>
auto installLight(Panel *panel, rack::engine::Module *module, float x, float y, int index) -> Light * {
  auto *light = rack::createLightCentered<Light>(mm2px(x, y), module, index);
  panel->addChild(light);
  return light;
}

template <typename P> class Jack : public rack::app::SvgPort {
public:
  Jack() { setSvg(controlSvg<P>("port")); }
};

template <typename Panel>
static auto installInput(Panel *panel, rack::engine::Module *module, float x, float y, int index) -> Jack<Panel> * {
  auto *input = rack::createInputCentered<Jack<Panel>>(mm2px(x, y), module, index);
  input->shadow->opacity = 0.F;
  panel->addInput(input);
  return input;
}

/**
 * Install an output port.
 */
template <typename Panel>
static auto installOutput(Panel *panel, rack::engine::Module *module, float x, float y, int index) -> Jack<Panel> * {
  auto *output = rack::createOutputCentered<Jack<Panel>>(mm2px(x, y), module, index);
  output->shadow->opacity = 0.F;
  panel->addOutput(output);
  return output;
}

} // namespace dhe
