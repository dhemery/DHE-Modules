#include <random>

#include <app.hpp>
#include <componentlibrary.hpp>
#include <engine.hpp>

#include "widget.h"

extern rack::Plugin *plugin;

namespace DHE {
Widget::Widget(rack::Module *module, int widget_hp, const char *background)
  : rack::ModuleWidget(module) {
  box.size = rack::Vec{(float) widget_hp*rack::RACK_GRID_WIDTH, rack::RACK_GRID_HEIGHT};

  auto *panel = new rack::SVGPanel();
  panel->box.size = box.size;
  panel->setBackground(rack::SVG::load(rack::assetPlugin(plugin, background)));
  addChild(panel);

  install_screws();
}

void Widget::install_screws() {
  static const float max_screw_inset{rack::RACK_GRID_WIDTH*1.5f};
  static constexpr float top_y{7.5f}; // Screw touches top and bottom of module

  auto left_x{std::min(width()/4, max_screw_inset)}; // One HP from edge
  auto right_x{width() - left_x};
  auto bottom_y{height() - top_y};

  auto screw_positions = std::vector<rack::Vec>{
      {left_x, top_y},
      {left_x, bottom_y},
      {right_x, top_y},
      {right_x, bottom_y}
  };

  std::random_device rd;
  auto screw_count = screw_positions.size();
  auto zero_through_three = std::uniform_int_distribution<unsigned long>{0, 3};

  auto black_screw_index = zero_through_three(rd);

  for (auto screw_index = 0UL; screw_index < screw_count; screw_index++) {
    if (screw_index==black_screw_index) {
      install_screw<rack::ScrewBlack>(screw_positions[screw_index]);
    } else {
      install_screw<rack::ScrewSilver>(screw_positions[screw_index]);
    }
  }
}

void Widget::moveTo(rack::Rect &box, rack::Vec pos) {
  box.pos = pos.minus(box.size.mult(0.5f));
}

void Widget::moveTo(rack::Rect &box, float x, float y) {
  moveTo(box, {x, y});
}
}
