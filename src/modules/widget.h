#ifndef DHE_MODULES_MODULES_WIDGET_H
#define DHE_MODULES_MODULES_WIDGET_H

#include <app.hpp>
#include <util/math.hpp>

namespace DHE {
class Widget : public rack::ModuleWidget {

public:
  Widget(rack::Module *module, int widget_hp, const char *background);

  float height() const {
    return box.size.y*MM_PER_IN/SVG_DPI;
  }

  float width() const {
    return box.size.x*MM_PER_IN/SVG_DPI;
  }

  template<class T>
  void install_button(int index, float x, float y, bool on = false) {
    install_param<T>(index, x, y, 0.f, 1.f, on ? 1.f : 0.f);
  }

  template<class T>
  void install_input(int index, float x, float y) {
    auto input{rack::Port::create<T>({0, 0}, rack::Port::INPUT, module, index)};
    moveTo(input->box, rack::mm2px({x, y}));
    addInput(input);
  }

  template<class T>
  void install_knob(int index, float x, float y) {
    install_param<T>(index, x, y, 0.f, 1.f, 0.5f);
  }

  template<class T>
  void install_light(int index, float x, float y, float initial = 0.f) {
    auto light{rack::ModuleLightWidget::create<T>({0, 0}, module, index)};
    moveTo(light->box, rack::mm2px({x, y}));
    addChild(light);
  }

  template<class T>
  void install_output(int index, float x, float y) {
    auto output{rack::Port::create<T>({0, 0}, rack::Port::OUTPUT, module, index)};
    moveTo(output->box, rack::mm2px({x, y}));
    addOutput(output);
  }

  template<class T>
  void install_param(int index, float x, float y, float low, float high, float initial) {
    auto param{rack::ParamWidget::create<T>({0, 0}, module, index, low, high, initial)};
    moveTo(param->box, rack::mm2px({x, y}));
    addParam(param);
  }

  template<class T>
  void install_screw(rack::Vec pos) {
    auto widget{rack::Widget::create<T>({0, 0})};
    moveTo(widget->box, rack::mm2px(pos));
    addChild(widget);
  }

  template<class T>
  void install_switch(int index, float x, float y, int max = 1, int initial = 0) {
    install_param<T>(index, x, y, 0.f, (float) max, (float) initial);
  }

  static void moveTo(rack::Rect &box, rack::Vec pos);

  void install_screws();
};

}
#endif
