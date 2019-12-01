#pragma once

#include "PanelAssets.h"

#include <app/SvgPort.hpp>
#include <helpers.hpp>

namespace dhe {

template <typename P> class Jack : public rack::app::SvgPort {
public:
  Jack() { setSvg(controlSvg<P>("port")); }
};

template <typename P> class InputJack : public Jack<P> {};
template <typename P> class OutputJack : public Jack<P> {};

template <typename Panel, typename Module>
static auto installInput(Panel *panel, Module *module, float x, float y, int index) -> InputJack<Panel> * {
  auto *input = rack::createInputCentered<InputJack<Panel>>(mm2px(x, y), module, index);
  input->shadow->opacity = 0.F;
  panel->addInput(input);
  return input;
}

template <typename Panel, typename Module>
static auto installOutput(Panel *panel, Module *module, float x, float y, int index) -> OutputJack<Panel> * {
  auto *output = rack::createOutputCentered<OutputJack<Panel>>(mm2px(x, y), module, index);
  output->shadow->opacity = 0.F;
  panel->addOutput(output);
  return output;
}

} // namespace dhe
