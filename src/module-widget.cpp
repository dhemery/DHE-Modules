#include <algorithm>
#include <random>

#include "module-widget.hpp"

namespace DHE {
void moveTo(rack::Rect &box, rack::Vec center) {
  box.pos = center.minus(box.size.mult(0.5f));
}

BooleanMenuItem::BooleanMenuItem(std::string name,
                                 std::function<void(bool)> set,
                                 std::function<bool()> is_on)
    : set{set}, is_on{is_on} {
  text = name;
}

void BooleanMenuItem::onAction(rack::EventAction &e) { set(!is_on()); }

void BooleanMenuItem::step() {
  rightText = is_on() ? "✔" : "";
  rack::MenuItem::step();
}

Port *Port::create(rack::Module *module, std::string module_name,
                   rack::Port::PortType type, int index, rack::Vec center) {
  auto image_file = std::string("res/") + module_name + "/port.svg";
  auto input = rack::Port::create<Port>({0, 0}, type, module, index);
  input->background->svg = rack::SVG::load(assetPlugin(plugin, image_file));
  input->background->wrap();
  input->box.size = input->background->box.size;
  moveTo(input->box, rack::mm2px(center));
  return input;
}

Knob *Knob::create(rack::Module *module, std::string module_name,
                   std::string size, int index, rack::Vec center,
                   float initial) {
  auto knob =
      rack::ParamWidget::create<Knob>({0, 0}, module, index, 0.f, 1.f, initial);
  auto image_file =
      std::string("res/") + module_name + "/knob-" + size + ".svg";
  knob->setSVG(rack::SVG::load(rack::assetPlugin(plugin, image_file)));
  knob->shadow->opacity = 0.f;
  moveTo(knob->box, rack::mm2px(center));
  return knob;
}

Button *Button::create(rack::Module *module, std::string module_name,
                       std::string type, int index, rack::Vec center) {
  auto button = Component::create<Button>({0, 0}, module);
  auto off_image_file =
      std::string("res/") + module_name + "/button-" + type + "-off.svg";
  auto on_image_file =
      std::string("res/") + module_name + "/button-" + type + "-on.svg";
  button->addFrame(rack::SVG::load(rack::assetPlugin(plugin, off_image_file)));
  button->addFrame(rack::SVG::load(rack::assetPlugin(plugin, on_image_file)));
  button->paramId = index;
  button->setLimits(0.f, 1.f);
  button->setDefaultValue(0.f);
  moveTo(button->box, rack::mm2px(center));
  return button;
}

Switch *Switch::create(rack::Module *module, std::string module_name, int index,
                       rack::Vec center, int max_position,
                       int initial_position) {
  auto widget = Component::create<Switch>({0, 0}, module);
  auto type = std::to_string(max_position + 1);
  auto low_image_file =
      std::string("res/") + module_name + "/switch-" + type + "-low.svg";
  widget->addFrame(rack::SVG::load(rack::assetPlugin(plugin, low_image_file)));
  if (max_position == 2) {
    auto mid_image_file =
        std::string("res/") + module_name + "/switch-" + type + "-mid.svg";
    widget->addFrame(
        rack::SVG::load(rack::assetPlugin(plugin, mid_image_file)));
  }
  auto high_image_file =
      std::string("res/") + module_name + "/switch-" + type + "-high.svg";
  widget->addFrame(rack::SVG::load(rack::assetPlugin(plugin, high_image_file)));
  widget->paramId = index;
  widget->setLimits(0.f, max_position);
  widget->setDefaultValue(initial_position);
  moveTo(widget->box, rack::mm2px(center));
  return widget;
}

ModuleWidget::ModuleWidget(rack::Module *module, int widget_hp,
                           std::string module_name)
    : rack::ModuleWidget(module), module_name{module_name} {
  box.size = rack::Vec{(float)widget_hp * rack::RACK_GRID_WIDTH,
                       rack::RACK_GRID_HEIGHT};

  auto *panel = new rack::SVGPanel();
  auto background = std::string("res/") + module_name + "/panel.svg";
  panel->box.size = box.size;
  panel->setBackground(rack::SVG::load(rack::assetPlugin(plugin, background)));
  addChild(panel);

  install_screws();
}

void ModuleWidget::install_screws() {

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

  install_screw<rack::ScrewBlack>(screw_positions.back());

  screw_positions.pop_back();

  for (auto p : screw_positions) {
    install_screw<rack::ScrewSilver>(p);
  }
}
} // namespace DHE
