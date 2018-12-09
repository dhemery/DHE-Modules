#pragma once
#include <algorithm>
#include <random>
#include <string>
#include <utility>

#include <app.hpp>
#include <util/math.hpp>

#include "dhe-modules.h"
#include "util/range.h"

namespace DHE {
inline void moveTo(rack::Rect &box, rack::Vec center) {
  box.pos = center.minus(box.size.mult(0.5f));
}


struct BooleanOption : rack::MenuItem {
  template<typename Setter, typename Getter>
  BooleanOption(const std::string &name, const Setter &setter,
                const Getter &getter)
      : set{setter}, is_on{getter} {
    text = name;
  }
  void onAction(rack::EventAction &e) override { set(!is_on()); }

  void step() override {
    rightText = is_on() ? "✔" : "";
    rack::MenuItem::step();
  }

  const std::function<void(bool)> set;
  const std::function<bool()> is_on;
};

struct ButtonWidget : rack::SVGSwitch, rack::MomentarySwitch {};
struct KnobWidget : rack::RoundKnob {};
struct PortWidget : rack::SVGPort {};
struct SwitchWidget : rack::SVGSwitch, rack::ToggleSwitch {};

template<typename TDisplay, typename TModule>
class ModuleWidget : public rack::ModuleWidget {

public:
  ModuleWidget(TModule *module, int widget_hp)
      : rack::ModuleWidget(module) {
    box.size = rack::Vec{(float) widget_hp*rack::RACK_GRID_WIDTH,
                         rack::RACK_GRID_HEIGHT};

    auto panel = new rack::SVGPanel();
    auto background = resource_prefix() + "/panel.svg";
    panel->box.size = box.size;
    panel->setBackground(rack::SVG::load(rack::assetPlugin(plugin, background)));
    addChild(panel);

    install_screws();
  }

  static auto resource_prefix() -> std::string {
    static const auto prefix = std::string("res/") + TDisplay::resource_name;
    return prefix;
  }

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
  auto height() const -> float { return box.size.y*MM_PER_IN/SVG_DPI; }

  auto width() const -> float { return box.size.x*MM_PER_IN/SVG_DPI; }

  void install_button(const std::string &type, int index, rack::Vec center) {
    addParam(create_button(type, index, center));
  }

  void install_input(int index, rack::Vec center) {
    addInput(create_port(rack::Port::INPUT, index, center));
  }

  void install_knob(const std::string &size, int index, rack::Vec center,
                    float initial_rotation = 0.5f) {
    addParam(create_knob(size, index, center, initial_rotation));
  }

  void install_output(int index, rack::Vec center) {
    addOutput(create_port(rack::Port::OUTPUT, index, center));
  }

  void install_switch(int index, rack::Vec center, int max_position = 1,
                      int initial_position = 0) {
    addParam(create_switch(index, center, max_position, initial_position));
  }

  void install_screws() {
    auto screw_diameter = rack::RACK_GRID_WIDTH*MM_PER_IN/SVG_DPI;
    auto screw_radius = screw_diameter/2.f;

    auto top = screw_radius;
    auto bottom = height() - top;

    auto max_screw_inset = screw_diameter*1.5f;
    auto left = std::min(width()/4.f, max_screw_inset);
    auto right = width() - left;

    auto screw_positions = std::vector<rack::Vec>{
        {left, top}, {left, bottom}, {right, top}, {right, bottom}};

    std::shuffle(screw_positions.begin(), screw_positions.end(),
                 std::mt19937(std::random_device()()));

    install_screw<rack::ScrewBlack>(screw_positions.back());

    screw_positions.pop_back();

    for (auto p : screw_positions) {
      install_screw<rack::ScrewSilver>(p);
    }
  }

  auto create_toggle(const std::string &type, int index, rack::Vec center, int max_position,
                     int initial_position = 0) -> SwitchWidget * {
    auto switch_widget = rack::Component::create<SwitchWidget>({0, 0}, module);
    for (int i = 0; i <= max_position; i++) {
      auto image_file = resource_prefix() + "/toggle-" + type + "-" + std::to_string(i) + ".svg";
      switch_widget->addFrame(rack::SVG::load(rack::assetPlugin(plugin, image_file)));
    }
    switch_widget->paramId = index;
    switch_widget->setLimits(0.f, max_position);
    switch_widget->setDefaultValue(initial_position);
    moveTo(switch_widget->box, rack::mm2px(center));
    return switch_widget;

  }

private:
  template<typename T> void install_screw(rack::Vec center) {
    auto widget = rack::Widget::create<T>({0, 0});
    moveTo(widget->box, rack::mm2px(center));
    addChild(widget);
  }

  auto create_button(const std::string &type, int index, rack::Vec center)
  -> ButtonWidget * {
    auto button_widget = rack::Component::create<ButtonWidget>({0, 0}, module);
    auto off_image_file = resource_prefix() + "/button-" + type + "-off.svg";
    auto on_image_file = resource_prefix() + "/button-" + type + "-on.svg";
    button_widget->addFrame(
        rack::SVG::load(rack::assetPlugin(plugin, off_image_file)));
    button_widget->addFrame(
        rack::SVG::load(rack::assetPlugin(plugin, on_image_file)));
    button_widget->paramId = index;
    button_widget->setLimits(0.f, 1.f);
    button_widget->setDefaultValue(0.f);
    moveTo(button_widget->box, rack::mm2px(center));
    return button_widget;
  }
  auto create_knob(const std::string &size, int index, rack::Vec center, float initial)
  -> KnobWidget * {
    auto knob_widget = rack::ParamWidget::create<KnobWidget>(
        {0, 0}, module, index, 0.f, 1.f, initial);
    auto image_file = resource_prefix() + "/knob-" + size + ".svg";
    knob_widget->setSVG(rack::SVG::load(rack::assetPlugin(plugin, image_file)));
    knob_widget->shadow->opacity = 0.f;
    moveTo(knob_widget->box, rack::mm2px(center));
    return knob_widget;
  }

  auto create_port(rack::Port::PortType type, int index, rack::Vec center)
  -> PortWidget * {
    auto image_file = resource_prefix() + "/port.svg";
    auto port_widget =
        rack::Port::create<PortWidget>({0, 0}, type, module, index);
    port_widget->background->svg =
        rack::SVG::load(assetPlugin(plugin, image_file));
    port_widget->background->wrap();
    port_widget->box.size = port_widget->background->box.size;
    moveTo(port_widget->box, rack::mm2px(center));
    return port_widget;
  }

  auto create_switch(int index, rack::Vec center, int max_position,
                     int initial_position = 0) -> SwitchWidget * {
    auto switch_widget = rack::Component::create<SwitchWidget>({0, 0}, module);
    auto type = std::to_string(max_position + 1);
    auto low_image_file = resource_prefix() + "/switch-" + type + "-low.svg";
    switch_widget->addFrame(
        rack::SVG::load(rack::assetPlugin(plugin, low_image_file)));
    if (max_position==2) {
      auto mid_image_file = resource_prefix() + "/switch-" + type + "-mid.svg";
      switch_widget->addFrame(
          rack::SVG::load(rack::assetPlugin(plugin, mid_image_file)));
    }
    auto high_image_file = resource_prefix() + "/switch-" + type + "-high.svg";
    switch_widget->addFrame(
        rack::SVG::load(rack::assetPlugin(plugin, high_image_file)));
    switch_widget->paramId = index;
    switch_widget->setLimits(0.f, max_position);
    switch_widget->setDefaultValue(initial_position);
    moveTo(switch_widget->box, rack::mm2px(center));
    return switch_widget;
  }
};

} // namespace DHE
