#pragma once

#include <string>
#include <app.hpp>
#include <util/math.hpp>

#include "dhe-modules.hpp"

#include "util/range.hpp"

namespace DHE {
void moveTo(rack::Rect &box, rack::Vec center);

struct Port : rack::SVGPort {
  static Port *create(rack::Module *module, std::string module_name, rack::Port::PortType type, int index, rack::Vec center) {
    auto image_file = std::string("res/") + module_name + "/port.svg";
    auto input = rack::Port::create<Port>({0, 0}, type, module, index);
    input->background->svg = rack::SVG::load(assetPlugin(plugin, image_file));
    input->background->wrap();
    input->box.size = input->background->box.size;
    moveTo(input->box, rack::mm2px(center));
    return input;
  }
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

  template<class T>
  void install_knob(int index, rack::Vec center, float initial_rotation = 0.5f) {
    install_param<T>(index, center, 0.f, 1.f, initial_rotation);
  }

  void install_output(int index, rack::Vec center) {
    auto output = Port::create(module, module_name, rack::Port::OUTPUT, index, center);
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

  void install_screws();
};

}
