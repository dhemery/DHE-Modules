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

  auto screw_diameter{rack::RACK_GRID_WIDTH*MM_PER_IN/SVG_DPI};
  auto screw_radius{screw_diameter/2.f};

  auto top{screw_radius};
  auto bottom{height() - top};

  auto max_screw_inset{screw_diameter*1.5f};
  auto left{std::min(width()/4.f, max_screw_inset)};
  auto right{width() - left};

  auto screw_positions = std::vector<rack::Vec>{
      {left, top},
      {left, bottom},
      {right, top},
      {right, bottom}
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

void Widget::moveTo(rack::Rect &box, rack::Vec center) {
  box.pos = center.minus(box.size.mult(0.5f));
}
}
