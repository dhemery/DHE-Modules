#ifndef DHE_GUI_WIDGET_H
#define DHE_GUI_WIDGET_H

#include <app.hpp>
#include <rack.hpp>
#include <util/math.hpp>

namespace DHE {
class Widget : public rack::ModuleWidget {

public:
  Widget(rack::Module *module, int widget_hp, const char *background);

  rack::Vec center() {
    return box.getCenter();
  }

  float height() const {
    return box.size.y;
  }

  float width() const {
    return box.size.x;
  }

  template<class T>
  void install_input(int index, float x, float y) {
    auto input{rack::Port::create<T>({x, y}, rack::Port::INPUT, module, index)};
    moveTo(input->box, x, y);
    addInput(input);
  }

  template<class T>
  void install_output(int index, float x, float y) {
    auto output{rack::Port::create<T>({0, 0}, rack::Port::OUTPUT, module, index)};
    moveTo(output->box, x, y);
    addOutput(output);
  }

  template<class T>
  void install_param(int index, float x, float y, float initial = 0.5f) {
    auto param{rack::ParamWidget::create<T>({x, y}, module, index, 0.0f, 1.0f, initial)};
    moveTo(param->box, x, y);
    addParam(param);
  }

  template<class T>
  void install_light(int index, float x, float y, float initial = 0.f) {
    auto light{rack::ModuleLightWidget::create<T>({x, y}, module, index)};
    moveTo(light->box, x, y);
    addChild(light);
  }

  template<class T>
  void install_button(int index, float x, float y) {
    install_param<T>(index, x, y, 0.0f);
  }

  template<class T>
  void install_screw(rack::Vec pos) {
    auto widget{rack::Widget::create<T>({0, 0})};
    moveTo(widget->box, pos);
    addChild(widget);
  }
  static void moveTo(rack::Rect &box, rack::Vec pos);

  static void moveTo(rack::Rect &box, float x, float y);

  void install_screws();
};

}
#endif
