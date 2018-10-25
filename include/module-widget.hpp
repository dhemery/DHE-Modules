#pragma once

#include <string>
#include <app.hpp>
#include <util/math.hpp>

#include "dhe-modules.hpp"

#include "util/range.hpp"

namespace DHE {
void moveTo(rack::Rect &box, rack::Vec center);

struct Port : rack::SVGPort {
  static Port *create(rack::Module *module, std::string module_name, rack::Port::PortType type, int index, rack::Vec center);
};

struct Knob : rack::RoundKnob {
  static Knob *create(rack::Module *module, std::string module_name, std::string size, int index, rack::Vec center, float initial);
};

struct Button : rack::SVGSwitch, rack::MomentarySwitch {
  static Button *create(rack::Module *module, std::string module_name, std::string type, int index, rack::Vec center);
};

class ModuleWidget : public rack::ModuleWidget {
  std::string module_name;

public:
  ModuleWidget(rack::Module *module, int widget_hp, std::string module_name);

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

  void install_input(int index, rack::Vec center) {
    auto input = Port::create(module, module_name, rack::Port::INPUT, index, center);
    addInput(input);
  }

  void install_knob(std::string size, int index, rack::Vec center, float initial_rotation = 0.5f) {
    auto knob = Knob::create(module, module_name, size, index, center, initial_rotation);
    addParam(knob);
  }

  void install_output(int index, rack::Vec center) {
    auto output = Port::create(module, module_name, rack::Port::OUTPUT, index, center);
    addOutput(output);
  }

  void install_button(std::string type, int index, rack::Vec center) {
    auto button = Button::create(module, module_name, type, index, center);
    addParam(button);
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

  void install_screws();
};

}
