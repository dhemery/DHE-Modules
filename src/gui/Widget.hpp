#pragma once

#include "app.hpp"
#include "math.hpp"
//#include "engine.hpp"

namespace DHE {
class Widget : public rack::ModuleWidget {

public:
  static constexpr float SCREW_DIAMETER = 15.0f;

  Widget(rack::Module *module, int widgetHP, const char *background);

  void createScrews(float horizontal_inset, float vertical_inset);

  template<class T>
  void installParam(int index, float x, float y) {
    rack::ParamWidget *param = rack::createParam<T>(rack::Vec(x, y), module, index, 0.0f, 1.0f, 0.5f);
    moveTo(param->box, x, y);
    addParam(param);
  }

  template<class T>
  void installInput(int index, float x, float y) {
    rack::Port *input = rack::createInput<T>(rack::Vec(x, y), module, index);
    moveTo(input->box, x, y);
    addInput(input);
  }

  template<class T>
  void installOutput(int index, float x, float y) {
    rack::Port *output = rack::createOutput<T>(rack::Vec(0, 0), module, index);
    moveTo(output->box, x, y);
    addOutput(output);
  }

  void moveTo(rack::Rect &box, float x, float y) {
    box.pos = rack::Vec(x, y).minus(box.size.div(2)); }
};

}
