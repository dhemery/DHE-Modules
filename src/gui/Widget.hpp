#pragma once

#include "app.hpp"
//#include "engine.hpp"

namespace DHE {
class Widget : public rack::ModuleWidget {

public:
  static constexpr float ROUND_BLACK_KNOB_DIAMETER = 38.0f;
  static constexpr float PORT_DIAMETER = 23.0f;
  static constexpr float PORT_RADIUS = PORT_DIAMETER/2.0f;
  static constexpr float SCREW_DIAMETER = 15.0f;

  Widget(rack::Module *module, int widgetWidth, const char *background);

  void createScrews(float horizontal_inset, float vertical_inset);

  template<class T>
  void installParam(int index, float x, float y) {
    addParam(rack::createParam<T>(rack::Vec(x, y), module, index, 0.0f, 1.0f, 0.5f));
  }

  template<class T>
  void installInput(int index, float x, float y) {
    addInput(rack::createInput<T>(rack::Vec(x, y), module, index));
  }

  template<class T>
  void installOutput(int index, float x, float y) {
    addOutput(rack::createOutput<T>(rack::Vec(x, y), module, index));
  }

};

}
