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
}

void Widget::install_screws(float leftX, float topY) {
  float rightX = width() - leftX;
  float bottomY = height() - topY;
  std::vector<rack::Vec> screwPositions{
      rack::Vec(leftX, topY),
      rack::Vec(leftX, bottomY),
      rack::Vec(rightX, topY),
      rack::Vec(rightX, bottomY)
  };

  unsigned long screwCount = screwPositions.size();
  std::minstd_rand generator;
  std::uniform_int_distribution<unsigned long> aScrew(0, 3);
  unsigned long blackScrew = aScrew(generator);

  for (unsigned long i = 0; i < screwCount; i++) {
    if (i==blackScrew) {
      install_screw<rack::ScrewBlack>(screwPositions[i]);
    } else {
      install_screw<rack::ScrewSilver>(screwPositions[i]);
    }
  }
}
}
