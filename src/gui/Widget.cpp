#include <random>
#include "asset.hpp"
#include "components.hpp"
#include "engine.hpp"
#include "math.hpp"
#include "plugin.hpp"
#include "rack.hpp"
#include "Widget.hpp"

extern rack::Plugin *plugin;

namespace DHE {
Widget::Widget(rack::Module *module, int width, const char *background) {
  setModule(module);
  box.size = rack::Vec(width*RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  auto *panel = new rack::SVGPanel();
  panel->box.size = box.size;
  panel->setBackground(rack::SVG::load(rack::assetPlugin(plugin, background)));
  addChild(panel);
}

void Widget::createScrews(float horizontal_inset, float vertical_inset) {
  float right = rack::Widget::box.size.x - horizontal_inset - SCREW_DIAMETER;
  float bottom = RACK_GRID_HEIGHT - vertical_inset - SCREW_DIAMETER;
  rack::Vec screwPositions[] = {
      rack::Vec(horizontal_inset, vertical_inset),
      rack::Vec(right, vertical_inset),
      rack::Vec(horizontal_inset, bottom),
      rack::Vec(right, bottom)};

  int screwCount = sizeof(screwPositions)/sizeof(*screwPositions);
  std::minstd_rand generator;
  std::uniform_int_distribution<int> aScrew(0, 3);
  int blackScrew = aScrew(generator);

  for (int i = 0; i < screwCount; i++) {
    if (i==blackScrew) {
      rack::Widget *screw = rack::createScrew<rack::ScrewBlack>(screwPositions[i]);
      rack::Widget::addChild(screw);
    } else
      rack::Widget::addChild(rack::createScrew<rack::ScrewSilver>(screwPositions[i]));
  }
}

}
