#pragma once

#include "Dimensions.h"

#include <app/SvgPort.hpp>
#include <engine/Module.hpp>
#include <helpers.hpp>
#include <math.hpp>
#include <widget/Widget.hpp>

namespace dhe {

static inline void positionCentered(rack::widget::Widget *widget, rack::math::Vec centermm) {
  widget->box.pos = mm2px(centermm).minus(widget->box.size.div(2));
}

static inline void positionCentered(rack::widget::Widget *widget, float xmm, float ymm) {
  positionCentered(widget, {xmm, ymm});
}

template <typename Widget, typename Panel> auto installWidget(Panel *panel, rack::math::Vec centermm) -> Widget * {
  auto *widget = rack::createWidget<Widget>({});
  positionCentered(widget, centermm);
  panel->addChild(widget);
  return widget;
}

template <typename Param, typename Panel>
auto installParam(Panel *panel, rack::engine::Module *module, float x, float y, int index) -> Param * {
  auto *param = rack::createParam<Param>({}, module, index);
  param->shadow->opacity = 0.F;
  param->setGraphics(Panel::moduleSlug, x, y);
  panel->addParam(param);
  return param;
}

template <typename Param, typename Panel>
static auto install(Panel *panel, rack::engine::Module *module, float x, float y, int index) -> Param * {
  return installParam<Param>(panel, module, x, y, index);
}

template <typename Light, typename Panel>
auto installLight(Panel *panel, rack::engine::Module *module, float x, float y, int index) -> Light * {
  auto *light = rack::createLight<Light>({}, module, index);
  positionCentered(light, x, y);
  panel->addChild(light);
  return light;
}

class Jack : public rack::app::SvgPort {
public:
  Jack() { shadow->opacity = 0.F; }
  void setGraphics(std::string const &moduleSlug, float x, float y) {
    setSvg(controlSvg(moduleSlug, "port"));
    positionCentered(this, x, y);
  }
};

template <typename Panel>
static auto installInput(Panel *panel, rack::engine::Module *module, float x, float y, int index) -> Jack * {
  auto *input = rack::createInput<Jack>({}, module, index);
  input->setGraphics(Panel::moduleSlug, x, y);
  panel->addInput(input);
  return input;
}

/**
 * Install an output port.
 */
template <typename Panel>
static auto installOutput(Panel *panel, rack::engine::Module *module, float x, float y, int index) -> Jack * {
  auto *output = rack::createOutput<Jack>({}, module, index);
  output->setGraphics(Panel::moduleSlug, x, y);
  panel->addOutput(output);
  return output;
}

} // namespace dhe
