#pragma once

#include <app.hpp>
#include <util/math.hpp>
#include <util/range.h>

#include "plugin/dhe-modules.h"

namespace DHE {
class ModuleWidget : public rack::ModuleWidget {

public:
  ModuleWidget(rack::Module *module, int widget_hp, const char *background);

  virtual void fromJson(json_t *patch) override {
    // If there's no data, we're loading from a legacy patch. Add empty data to
    // the incoming patch so that ModuleWidget::fromJson will call
    // Module::fromJson, which will configure the module with appropriate legacy
    // behavior.
    if (!json_object_get(patch, "data")) {
      json_object_set_new(patch, "data", json_object());
    }
    rack::ModuleWidget::fromJson(patch);
  }

  float height() const {
    return box.size.y*MM_PER_IN/SVG_DPI;
  }

  float width() const {
    return box.size.x*MM_PER_IN/SVG_DPI;
  }

  template<class T>
  void install_input(int index, rack::Vec center) {
    auto input = rack::Port::create<T>({0, 0}, rack::Port::INPUT, module, index);
    moveTo(input->box, rack::mm2px(center));
    addInput(input);
  }

  template<class T>
  void install_knob(int index, rack::Vec center, float initial_rotation = 0.5f) {
    install_param<T>(index, center, 0.f, 1.f, initial_rotation);
  }

  template<class T>
  void install_output(int index, rack::Vec center) {
    auto output = rack::Port::create<T>({0, 0}, rack::Port::OUTPUT, module, index);
    moveTo(output->box, rack::mm2px(center));
    addOutput(output);
  }

  template<class T>
  void install_param(int index, rack::Vec center, float low, float high, float initial) {
    auto param = rack::ParamWidget::create<T>({0, 0}, module, index, low, high, initial);
    moveTo(param->box, rack::mm2px(center));
    addParam(param);
  }

  template<class T>
  void install_screw(rack::Vec center) {
    auto widget = rack::Widget::create<T>({0, 0});
    moveTo(widget->box, rack::mm2px(center));
    addChild(widget);
  }

  template<class T>
  void install_switch(int index, rack::Vec center, int max_position = 1, int initial_position = 0) {
    install_param<T>(index, center, 0.f, (float) max_position, (float) initial_position);
  }

  static void moveTo(rack::Rect &box, rack::Vec center);

  void install_screws();
};

}
