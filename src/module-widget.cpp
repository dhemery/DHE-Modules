#include <algorithm>
#include <random>

#include "module-widget.hpp"

namespace DHE {
void moveTo(rack::Rect &box, rack::Vec center) {
  box.pos = center.minus(box.size.mult(0.5f));
}

void BooleanOption::onAction(rack::EventAction &e) { set(!is_on()); }

void BooleanOption::step() {
  rightText = is_on() ? "✔" : "";
  rack::MenuItem::step();
}

ModuleWidget::ModuleWidget(rack::Module *module, int widget_hp,
                           const std::string &module_name)
    : rack::ModuleWidget(module), module_name_{module_name} {
  box.size = rack::Vec{(float)widget_hp * rack::RACK_GRID_WIDTH,
                       rack::RACK_GRID_HEIGHT};

  auto panel = new rack::SVGPanel();
  auto background = std::string("res/") + module_name + "/panel.svg";
  panel->box.size = box.size;
  panel->setBackground(rack::SVG::load(rack::assetPlugin(plugin, background)));
  addChild(panel);

  install_screws();
}

auto ModuleWidget::create_button(std::string type, int index, rack::Vec center)
    -> ButtonWidget * {
  auto button_widget = rack::Component::create<ButtonWidget>({0, 0}, module);
  auto off_image_file =
      std::string("res/") + module_name_ + "/button-" + type + "-off.svg";
  auto on_image_file =
      std::string("res/") + module_name_ + "/button-" + type + "-on.svg";
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

auto ModuleWidget::create_knob(std::string size, int index, rack::Vec center,
                               float initial) -> KnobWidget * {
  auto knob_widget = rack::ParamWidget::create<KnobWidget>(
      {0, 0}, module, index, 0.f, 1.f, initial);
  auto image_file =
      std::string("res/") + module_name_ + "/knob-" + size + ".svg";
  knob_widget->setSVG(rack::SVG::load(rack::assetPlugin(plugin, image_file)));
  knob_widget->shadow->opacity = 0.f;
  moveTo(knob_widget->box, rack::mm2px(center));
  return knob_widget;
}

auto ModuleWidget::create_port(rack::Port::PortType type, int index,
                               rack::Vec center) -> PortWidget * {
  auto image_file = std::string("res/") + module_name_ + "/port.svg";
  auto port_widget =
      rack::Port::create<PortWidget>({0, 0}, type, module, index);
  port_widget->background->svg =
      rack::SVG::load(assetPlugin(plugin, image_file));
  port_widget->background->wrap();
  port_widget->box.size = port_widget->background->box.size;
  moveTo(port_widget->box, rack::mm2px(center));
  return port_widget;
}

auto ModuleWidget::create_switch(int index, rack::Vec center, int max_position,
                                 int initial_position) -> SwitchWidget * {
  auto switch_widget = rack::Component::create<SwitchWidget>({0, 0}, module);
  auto type = std::to_string(max_position + 1);
  auto low_image_file =
      std::string("res/") + module_name_ + "/switch-" + type + "-low.svg";
  switch_widget->addFrame(
      rack::SVG::load(rack::assetPlugin(plugin, low_image_file)));
  if (max_position == 2) {
    auto mid_image_file =
        std::string("res/") + module_name_ + "/switch-" + type + "-mid.svg";
    switch_widget->addFrame(
        rack::SVG::load(rack::assetPlugin(plugin, mid_image_file)));
  }
  auto high_image_file =
      std::string("res/") + module_name_ + "/switch-" + type + "-high.svg";
  switch_widget->addFrame(
      rack::SVG::load(rack::assetPlugin(plugin, high_image_file)));
  switch_widget->paramId = index;
  switch_widget->setLimits(0.f, max_position);
  switch_widget->setDefaultValue(initial_position);
  moveTo(switch_widget->box, rack::mm2px(center));
  return switch_widget;
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
