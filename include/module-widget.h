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

class SVGLoader {
public:
  explicit SVGLoader(const std::string &module_dir) : resource_dir{resource_dir_for(module_dir)} {}

  auto load_svg(const std::string &filename) -> std::shared_ptr<rack::SVG> {
    return rack::SVG::load(resource_dir + filename + ".svg");
  }

private:
  static auto resource_dir_for(const std::string &module_dir) -> std::string {
    static const auto plugin_resource_root = rack::assetPlugin(plugin, "res/");
    return plugin_resource_root + module_dir + "/";
  }
  const std::string resource_dir;
};

class Potentiometer : public rack::RoundKnob, SVGLoader {
public:
  Potentiometer(const std::string &module_dir, const std::string &size) : SVGLoader{module_dir} {
    static const auto prefix = std::string{"knob-"};
    setSVG(load_svg(prefix + size));
    shadow->opacity = 0.f;
  }
};

class LargeKnob : public Potentiometer {
public:
  explicit LargeKnob(const std::string &module_dir) : Potentiometer(module_dir, "large") {}
};

class MediumKnob : public Potentiometer {
public:
  explicit MediumKnob(const std::string &module_dir) : Potentiometer(module_dir, "medium") {}
};

class SmallKnob : public Potentiometer {
public:
  explicit SmallKnob(const std::string &module_dir) : Potentiometer(module_dir, "small") {}
};

class TinyKnob : public Potentiometer {
public:
  explicit TinyKnob(const std::string &module_dir) : Potentiometer(module_dir, "tiny") {}
};

template<typename TDisplay>
class NormalButton : public rack::SVGSwitch, public rack::MomentarySwitch {
public:
  NormalButton() {
    addFrame(TDisplay::svg("button-normal-1"));
    addFrame(TDisplay::svg("button-normal-2"));
  }

  static auto create(rack::Module *module, int index) -> NormalButton * {
    return rack::ParamWidget::create<NormalButton<TDisplay>>({0, 0}, module, index, 0, 1, 0);
  }
};

template<typename TDisplay>
class ReverseButton : public rack::SVGSwitch, public rack::MomentarySwitch {
public:
  ReverseButton() {
    addFrame(TDisplay::svg("button-reverse-1"));
    addFrame(TDisplay::svg("button-reverse-2"));
  }

  static auto create(rack::Module *module, int index) -> ReverseButton * {
    return rack::ParamWidget::create<ReverseButton<TDisplay>>({0, 0}, module, index, 0, 1, 0);
  }
};

template<typename TDisplay>
class ThumbSwitch2 : public rack::SVGSwitch, public rack::ToggleSwitch {
public:
  ThumbSwitch2() {
    addFrame(TDisplay::svg("thumb-2-1"));
    addFrame(TDisplay::svg("thumb-2-2"));
  }

  static auto create(rack::Module *module, int index, int initial_position = 0) -> ThumbSwitch2 * {
    return rack::ParamWidget::create<ThumbSwitch2<TDisplay>>({0, 0}, module, index, 0, 1, initial_position);
  }

};

template<typename TDisplay>
class ThumbSwitch3 : public rack::SVGSwitch, public rack::ToggleSwitch {
public:
  ThumbSwitch3() {
    addFrame(TDisplay::svg("thumb-3-1"));
    addFrame(TDisplay::svg("thumb-3-2"));
    addFrame(TDisplay::svg("thumb-3-3"));
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

  template<typename TKnob>
  auto knob(int index, float initial = 0.5f) const -> Potentiometer * {
    return rack::ParamWidget::create<TKnob>({0,0}, module, index, 0, 1, initial);
  }

  auto button(int index) const -> NormalButton<TDisplay> * {
    return NormalButton<TDisplay>::create(module, index);
  }

  auto reverse_button(int index) const -> ReverseButton<TDisplay> * {
    return ReverseButton<TDisplay>::create(module, index);
  }

  auto thumb_switch_2(int index, int initial = 0) const -> ThumbSwitch2<TDisplay> * {
    return ThumbSwitch2<TDisplay>::create(module, index, initial);
  }

  auto thumb_switch_3(int index, int initial = 0) const -> ThumbSwitch3<TDisplay> * {
    return ThumbSwitch3<TDisplay>::create(module, index, initial);
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

    auto p_special = screw_positions.back();
    install(p_special.x, p_special.y, rack::Widget::create<rack::ScrewBlack>());

    screw_positions.pop_back();

    for (auto p : screw_positions) {
      install(p.x, p.y, rack::Widget::create<rack::ScrewSilver>());
    }
  }

private:
  static auto resource_prefix() -> std::string {
    static const auto prefix = std::string("res/") + TDisplay::resource_name + "/";
    return prefix;
  }
};

} // namespace DHE
