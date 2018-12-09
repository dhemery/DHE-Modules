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

inline void moveTo(float x, float y, rack::Widget *widget) {
  moveTo(widget->box, rack::mm2px({x, y}));
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
struct SwitchWidget : rack::SVGSwitch, rack::ToggleSwitch {};

template<typename TDisplay>
class ThumbSwitch2 : public rack::SVGSwitch, public rack::ToggleSwitch {
public:
  ThumbSwitch2() {
    addFrame(TDisplay::svg("switch-2-low"));
    addFrame(TDisplay::svg("switch-2-low"));
  }

  static auto create(rack::Module *module, int index, int initial_position = 0) -> ThumbSwitch2 * {
    return rack::ParamWidget::create<ThumbSwitch2<TDisplay>>({0, 0}, module, index, 0, 1, initial_position);
  }

};

template<typename TDisplay>
class ThumbSwitch3 : public rack::SVGSwitch, public rack::ToggleSwitch {
public:
  ThumbSwitch3() {
    addFrame(TDisplay::svg("switch-3-low"));
    addFrame(TDisplay::svg("switch-3-mid"));
    addFrame(TDisplay::svg("switch-3-high"));
  }

  static auto create(rack::Module *module, int index, int initial_position = 0) -> ThumbSwitch3 * {
    return rack::ParamWidget::create<ThumbSwitch3<TDisplay>>({0, 0}, module, index, 0, 2, initial_position);
  }
};

template<typename TDisplay>
class Jack : public rack::SVGPort {
public:
  Jack() {
    background->svg = TDisplay::svg("port");
    background->wrap();
    box.size = background->box.size;
  }
};

template<typename TDisplay>
class InputJack : public Jack<TDisplay> {
public:
  static auto create(rack::Module *module, int index) -> InputJack * {
    return rack::Port::create<InputJack<TDisplay>>({0, 0}, rack::Port::PortType::INPUT, module, index);
  }
};

template<typename TDisplay>
class OutputJack : public Jack<TDisplay> {
public:
  static auto create(rack::Module *module, int index) -> OutputJack * {
    return rack::Port::create<OutputJack<TDisplay>>({0, 0}, rack::Port::PortType::OUTPUT, module, index);
  }

};

template<typename TDisplay>
class Potentiometer : public rack::RoundKnob {
public:
  static auto create_tiny(rack::Module *module, int index, float initial) -> Potentiometer * {
    return create(module, "knob-tiny", index, initial);
  }

  static auto create_small(rack::Module *module, int index, float initial) -> Potentiometer * {
    return create(module, "knob-small", index, initial);
  }

  static auto create_medium(rack::Module *module, int index, float initial) -> Potentiometer * {
    return create(module, "knob-medium", index, initial);
  }

  static auto create_large(rack::Module *module, int index, float initial) -> Potentiometer * {
    return create(module, "knob-large", index, initial);
  }

private:
  static auto create(rack::Module *module, std::string file, int index, float initial) -> Potentiometer * {
    auto potentiometer = rack::ParamWidget::create<Potentiometer<TDisplay>>(
        {0, 0}, module, index, 0.f, 1.f, initial);
    potentiometer->setSVG(TDisplay::svg(file));
    potentiometer->shadow->opacity = 0.f;
    return potentiometer;
  }
};

template<typename TDisplay, typename TModule>
class ModuleWidget : public rack::ModuleWidget {

public:
  ModuleWidget(TModule *module, int widget_hp)
      : rack::ModuleWidget(module) {
    box.size = rack::Vec{(float) widget_hp*rack::RACK_GRID_WIDTH,
                         rack::RACK_GRID_HEIGHT};

    auto panel = new rack::SVGPanel();
    panel->box.size = box.size;
    panel->setBackground(TDisplay::svg("panel"));
    addChild(panel);

    install_screws();
  }

  static auto resource_prefix() -> std::string {
    static const auto prefix = std::string("res/") + TDisplay::resource_name + "/";
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

  static auto svg(const std::string &filename) -> std::shared_ptr<rack::SVG> {
    static const auto asset_dir = rack::assetPlugin(plugin, resource_prefix());
    return rack::SVG::load(asset_dir + filename + ".svg");
  }

protected:
  auto height() const -> float { return box.size.y*MM_PER_IN/SVG_DPI; }

  auto width() const -> float { return box.size.x*MM_PER_IN/SVG_DPI; }

  void install(float x, float y, InputJack<TDisplay> *jack) {
    moveTo(x, y, jack);
    addInput(jack);
  }

  void install(float x, float y, rack::ParamWidget *widget) {
    moveTo(x, y, widget);
    addParam(widget);
  }

  void install(float x, float y, OutputJack<TDisplay> *jack) {
    moveTo(x, y, jack);
    addOutput(jack);
  }

  void install_button(const std::string &type, int index, rack::Vec center) {
    addParam(create_button(type, index, center));
  }

  auto input_jack(int index) const -> InputJack<TDisplay> * {
    return InputJack<TDisplay>::create(module, index);
  }

  auto output_jack(int index) const -> OutputJack<TDisplay> * {
    return OutputJack<TDisplay>::create(module, index);
  }

  auto thumb_switch_2(int index, int initial = 0) const -> ThumbSwitch2<TDisplay> * {
    return ThumbSwitch2<TDisplay>::create(module, index, initial);
  }

  auto thumb_switch_3(int index, int initial = 0) const -> ThumbSwitch3<TDisplay> * {
    return ThumbSwitch3<TDisplay>::create(module, index, initial);
  }

  auto tiny_knob(int index, float initial_rotation = 0.5f) const -> Potentiometer <TDisplay> * {
    return Potentiometer<TDisplay>::create_tiny(module, index, initial_rotation);
  }

  auto small_knob(int index, float initial_rotation = 0.5f) const -> Potentiometer <TDisplay> * {
    return Potentiometer<TDisplay>::create_small(module, index, initial_rotation);
  }

  auto medium_knob(int index, float initial_rotation = 0.5f) const -> Potentiometer <TDisplay> * {
    return Potentiometer<TDisplay>::create_medium(module, index, initial_rotation);
  }

  auto large_knob(int index, float initial_rotation = 0.5f) const -> Potentiometer <TDisplay> * {
    return Potentiometer<TDisplay>::create_large(module, index, initial_rotation);
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
