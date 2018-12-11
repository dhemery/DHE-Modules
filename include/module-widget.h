#pragma once
#include <algorithm>
#include <random>
#include <string>
#include <utility>

#include <app.hpp>
#include <util/math.hpp>

#include "dhe-modules.h"

namespace DHE {
inline void moveTo(rack::Rect &box, rack::Vec center) {
  box.pos = center.minus(box.size.mult(0.5f));
}

inline void moveTo(float x, float y, rack::Widget *widget) {
  moveTo(widget->box, rack::mm2px({x, y}));
}

struct BooleanOption : rack::MenuItem {
  template <typename Setter, typename Getter>
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

template <typename TDisplay> class Jack : public rack::SVGPort {
public:
  Jack() {
    background->svg = TDisplay::svg("port");
    background->wrap();
    box.size = background->box.size;
  }
};

template <typename TDisplay> class InputJack : public Jack<TDisplay> {
public:
  static auto create(rack::Module *module, int index) -> InputJack * {
    return rack::Port::create<InputJack<TDisplay>>(
        {0, 0}, rack::Port::PortType::INPUT, module, index);
  }
};

template <typename TDisplay> class OutputJack : public Jack<TDisplay> {
public:
  static auto create(rack::Module *module, int index) -> OutputJack * {
    return rack::Port::create<OutputJack<TDisplay>>(
        {0, 0}, rack::Port::PortType::OUTPUT, module, index);
  }
};

template <typename TDisplay, typename TModule>
class ModuleWidget : public rack::ModuleWidget {

public:
  ModuleWidget(TModule *module, int widget_hp) : rack::ModuleWidget(module) {
    box.size = rack::Vec{(float)widget_hp * rack::RACK_GRID_WIDTH,
                         rack::RACK_GRID_HEIGHT};

    auto panel = new rack::SVGPanel();
    panel->box.size = box.size;
    panel->setBackground(TDisplay::svg("panel"));
    addChild(panel);

    install_screws();
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
  auto height() const -> float { return box.size.y * MM_PER_IN / SVG_DPI; }

  auto width() const -> float { return box.size.x * MM_PER_IN / SVG_DPI; }

  void install(float x, float y, rack::ParamWidget *widget) {
    moveTo(x, y, widget);
    addParam(widget);
  }

  void install(float x, float y, rack::Widget *widget) {
    moveTo(x, y, widget);
    addChild(widget);
  }

  void install(float x, float y, InputJack<TDisplay> *jack) {
    moveTo(x, y, jack);
    addInput(jack);
  }

  void install(float x, float y, OutputJack<TDisplay> *jack) {
    moveTo(x, y, jack);
    addOutput(jack);
  }

  auto input_jack(int index) const -> InputJack<TDisplay> * {
    return InputJack<TDisplay>::create(module, index);
  }

  auto output_jack(int index) const -> OutputJack<TDisplay> * {
    return OutputJack<TDisplay>::create(module, index);
  }

  template <typename TKnob>
  auto knob(int index, float initial = 0.5f) const -> TKnob * {
    return rack::ParamWidget::create<TKnob>({0, 0}, module, index, 0, 1,
                                            initial);
  }

  template <typename TButton> auto button(int index) const -> TButton * {
    return rack::ParamWidget::create<TButton>({0, 0}, module, index, 0, 1, 0);
  }

  template <typename TSwitch>
  auto thumb_switch(int index, int initial = 0) const -> TSwitch * {
    return rack::ParamWidget::create<TSwitch>({0, 0}, module, index, 0,
                                              TSwitch::size - 1, initial);
  }

  void install_screws() {
    auto screw_diameter = rack::RACK_GRID_WIDTH * MM_PER_IN / SVG_DPI;
    auto screw_radius = screw_diameter / 2.f;

    auto top = screw_radius;
    auto bottom = height() - top;

    auto max_screw_inset = screw_diameter * 1.5f;
    auto left = std::min(width() / 4.f, max_screw_inset);
    auto right = width() - left;

    auto screw_positions = std::vector<rack::Vec>{
        {left, top}, {left, bottom}, {right, top}, {right, bottom}};

    std::shuffle(screw_positions.begin(), screw_positions.end(),
                 std::mt19937(std::random_device()()));

    auto p_special = screw_positions.back();
    install(p_special.x, p_special.y, rack::Widget::create<rack::ScrewBlack>());

    screw_positions.pop_back();

    for (auto p : screw_positions) {
      install(p.x, p.y, rack::Widget::create<rack::ScrewSilver>());
    }
  }

private:
  static auto resource_prefix() -> std::string {
    static const auto prefix =
        std::string("res/") + TDisplay::resource_name + "/";
    return prefix;
  }
};

} // namespace DHE
