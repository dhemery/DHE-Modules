#include <utility>

#include <utility>

#pragma once

#include <app.hpp>
#include <string>
#include <util/math.hpp>

#include "dhe-modules.hpp"

#include "util/range.hpp"

namespace DHE {
void moveTo(rack::Rect &box, rack::Vec center);

struct BooleanOption : rack::MenuItem {
  template <typename Setter, typename Getter>
  BooleanOption(std::string const &name, Setter const &setter,
                Getter const &getter)
      : set_{setter}, is_on_{getter} {
    text = name;
  }
  void onAction(rack::EventAction &e) override;
  void step() override;

  std::function<void(bool)> const set_;
  std::function<bool()> const is_on_;
};

struct ButtonWidget : rack::SVGSwitch, rack::MomentarySwitch {};
struct KnobWidget : rack::RoundKnob {};
struct PortWidget : rack::SVGPort {};
struct SwitchWidget : rack::SVGSwitch, rack::ToggleSwitch {};

class ModuleWidget : public rack::ModuleWidget {
  std::string const module_name_;

public:
  ModuleWidget(rack::Module *module, int widget_hp,
               std::string const &module_name);

  void fromJson(json_t *patch) override {
    // If there's no data, we're loading from a legacy patch. Add empty data to
    // the incoming patch so that ModuleWidget::fromJson will call
    // Module::fromJson, which will configure the module with appropriate legacy
    // behavior.
    if (!json_object_get(patch, "data")) {
      json_object_set_new(patch, "data", json_object());
    }
    rack::ModuleWidget::fromJson(patch);
  }

protected:
  auto height() const -> float { return box.size.y * MM_PER_IN / SVG_DPI; }

  auto width() const -> float { return box.size.x * MM_PER_IN / SVG_DPI; }

  void install_button(std::string type, int index, rack::Vec center) {
    addParam(create_button(std::move(type), index, center));
  }

  void install_input(int index, rack::Vec center) {
    addInput(create_port(rack::Port::INPUT, index, center));
  }

  void install_knob(std::string size, int index, rack::Vec center,
                    float initial_rotation = 0.5f) {
    addParam(create_knob(std::move(size), index, center, initial_rotation));
  }

  void install_output(int index, rack::Vec center) {
    addOutput(create_port(rack::Port::OUTPUT, index, center));
  }

  void install_switch(int index, rack::Vec center, int max_position = 1,
                      int initial_position = 0) {
    addParam(create_switch(index, center, max_position, initial_position));
  }

  void install_screws();

private:
  template <typename T> void install_screw(rack::Vec center) {
    auto widget{rack::Widget::create<T>({0, 0})};
    moveTo(widget->box, rack::mm2px(center));
    addChild(widget);
  }

  auto create_button(std::string type, int index, rack::Vec center)
      -> ButtonWidget *;
  auto create_knob(std::string size, int index, rack::Vec center, float initial)
      -> KnobWidget *;
  auto create_port(rack::Port::PortType type, int index, rack::Vec center)
      -> PortWidget *;
  auto create_switch(int index, rack::Vec center, int high_position,
                     int initial_position = 0) -> SwitchWidget *;
};

} // namespace DHE
