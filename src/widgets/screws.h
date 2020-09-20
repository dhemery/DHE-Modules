#pragma once

#include "dimensions.h"

#include <app/ModuleWidget.hpp>
#include <componentlibrary.hpp>
#include <helpers.hpp>
#include <random>

namespace dhe {

using NormalScrew = rack::componentlibrary::ScrewSilver;
using SpecialScrew = rack::componentlibrary::ScrewBlack;

static inline void install_screws(rack::app::ModuleWidget *panel, int hp) {
  auto constexpr screw_diameter = hp2mm(1.F);
  auto constexpr screw_radius = screw_diameter / 2.F;
  auto const panel_width = hp2mm((float)hp);

  auto constexpr top = screw_radius;
  auto constexpr bottom = module_height - screw_radius;

  auto const left = hp < 3 ? screw_radius : screw_radius + screw_diameter;
  auto const right = panel_width - left;

  auto screw_positions =
      std::vector<rack::math::Vec>{mm2px(left, top), mm2px(right, bottom)};
  if (hp > 4) {
    screw_positions.push_back(mm2px(right, top));
    screw_positions.push_back(mm2px(left, bottom));
  }

  std::shuffle(screw_positions.begin(), screw_positions.end(),
               std::mt19937(std::random_device()()));

  auto const position_of_special_screw = screw_positions.back();
  panel->addChild(
      rack::createWidgetCentered<SpecialScrew>(position_of_special_screw));

  screw_positions.pop_back();

  for (auto const &screw_position : screw_positions) {
    panel->addChild(rack::createWidgetCentered<NormalScrew>(screw_position));
  }
}

} // namespace dhe
