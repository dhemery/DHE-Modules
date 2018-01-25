#pragma once

#include "app.hpp"
#include "engine.hpp"
#include "math.hpp"
#include "rack.hpp"

namespace DHE {
class Widget : public rack::ModuleWidget {

public:
  Widget(rack::Module *module, int widgetHP, const char *background);

  rack::Vec center() {
    return box.getCenter();
  }

  float height() {
    return box.size.y;
  }

  float width() {
    return box.size.x;
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

  template<class T>
  void installParam(int index, float x, float y) {
    rack::ParamWidget *param = rack::createParam<T>(rack::Vec(x, y), module, index, 0.0f, 1.0f, 0.5f);
    moveTo(param->box, x, y);
    addParam(param);
  }
  template<class T>
  void installScrew(rack::Vec pos) {
    rack::Widget *widget = rack::createScrew<T>(rack::Vec(0, 0));
    moveTo(widget->box, pos);
    addChild(widget);

  }

  void installScrews(float leftX, float topY);

  static void moveTo(rack::Rect &box, rack::Vec pos) {
    box.pos = pos.minus(box.size.div(2));
  }

  static void moveTo(rack::Rect &box, float x, float y) {
    moveTo(box, rack::Vec(x, y));
  }
};

}
