#ifndef DHE_GUI_WIDGET_H
#define DHE_GUI_WIDGET_H

#include "app.hpp"
#include "engine.hpp"
#include "math.hpp"
#include "rack.hpp"

namespace DHE {
static constexpr float MAX_SCREW_INSET{RACK_GRID_WIDTH*1.5f};

class Widget : public rack::ModuleWidget {

public:

  Widget(rack::Module *module, int widget_hp, const char *background);

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
  void install_input(int index, float x, float y) {
    auto input{rack::createInput<T>(rack::Vec(x, y), module, index)};
    moveTo(input->box, x, y);
    addInput(input);
  }

  template<class T>
  void install_output(int index, float x, float y) {
    auto output{rack::createOutput<T>(rack::Vec(0, 0), module, index)};
    moveTo(output->box, x, y);
    addOutput(output);
  }

  template<class T>
  void install_param(int index, float x, float y, float initial = 0.5f) {
    auto param{rack::createParam<T>(rack::Vec(x, y), module, index, 0.0f, 1.0f, initial)};
    moveTo(param->box, x, y);
    addParam(param);
  }

  template<class T>
  void install_light(int index, float x, float y, float initial = 0.5f) {
    auto light{rack::createLight<T>(rack::Vec(x, y), module, index)};
    moveTo(light->box, x, y);
    addChild(light);
  }

  template<class T>
  void install_button(int index, float x, float y) {
    install_param<T>(index, x, y, 0.0f);
  }

  template<class T>
  void install_screw(rack::Vec pos) {
    auto widget{rack::createScrew<T>(rack::Vec(0, 0))};
    moveTo(widget->box, pos);
    addChild(widget);
  }

  void install_screws();

  static void moveTo(rack::Rect &box, rack::Vec pos) {
    box.pos = pos.minus(box.size.div(2));
  }

  static void moveTo(rack::Rect &box, float x, float y) {
    moveTo(box, rack::Vec(x, y));
  }
};

}
#endif
