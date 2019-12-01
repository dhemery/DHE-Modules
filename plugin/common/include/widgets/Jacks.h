#pragma once

#include "PanelAssets.h"

#include <app/SvgPort.hpp>
#include <helpers.hpp>

namespace dhe {

template <typename P> class Jack : public rack::app::SvgPort {
public:
  Jack() { setSvg(controlSvg<P>("port")); }
};

template <typename P> class InputJack : public Jack<P> {
public:
  template <typename Panel, typename Module>
  static auto install(Panel *panel, Module *module, float x, float y, int index) -> InputJack<P> * {
    auto *input = rack::createInputCentered<InputJack<P>>(mm2px(x, y), module, index);
    input->shadow->opacity = 0.F;
    panel->addInput(input);
    return input;
  }
};

template <typename P> class OutputJack : public Jack<P> {
public:
  template <typename Panel, typename Module>
  static auto install(Panel *panel, Module *module, float x, float y, int index) -> OutputJack<P> * {
    auto *output = rack::createOutputCentered<OutputJack<P>>(mm2px(x, y), module, index);
    output->shadow->opacity = 0.F;
    panel->addOutput(output);
    return output;
  }
};

} // namespace dhe
