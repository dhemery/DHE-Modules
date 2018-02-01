#include <random>
#include "asset.hpp"
#include "components.hpp"
#include "engine.hpp"
#include "math.hpp"
#include "plugin.hpp"
#include "rack.hpp"
#include "widget.h"

extern rack::Plugin *plugin;

namespace DHE {
Widget::Widget(rack::Module *module, int widget_hp, const char *background) {
  setModule(module);
  box.size = rack::Vec(widget_hp*RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  auto *panel = new rack::SVGPanel();
  panel->box.size = box.size;
  panel->setBackground(rack::SVG::load(rack::assetPlugin(plugin, background)));
  addChild(panel);

  install_screws();
}

void Widget::install_screws() {
  auto left_x{std::min(width() / 4, MAX_SCREW_INSET)}; // One HP from edge
  auto top_y{7.5f}; // Screw touches top of module

  auto right_x = width() - left_x;
  auto bottom_y = height() - top_y;
  std::vector<rack::Vec> screw_positions{
      rack::Vec(left_x, top_y),
      rack::Vec(left_x, bottom_y),
      rack::Vec(right_x, top_y),
      rack::Vec(right_x, bottom_y)
  };

  auto screw_count = screw_positions.size();
  std::random_device rd;
  auto zero_through_three = std::uniform_int_distribution<unsigned long>{0, 3};

  auto black_screw_index = zero_through_three(rd);

  for (auto i = 0UL; i < screw_count; i++) {
    if (i==black_screw_index) {
      install_screw<rack::ScrewBlack>(screw_positions[i]);
    } else {
      install_screw<rack::ScrewSilver>(screw_positions[i]);
    }
  }
}
}
