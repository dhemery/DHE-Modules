#pragma once

#include "Dimensions.h"

#include <app/ModuleWidget.hpp>
#include <componentlibrary.hpp>
#include <helpers.hpp>
#include <random>

namespace dhe {

using NormalScrew = rack::componentlibrary::ScrewSilver;
using SpecialScrew = rack::componentlibrary::ScrewBlack;

static inline void installScrews(rack::app::ModuleWidget *panel, int hp) {
  auto constexpr screwDiameter = hp2mm(1.F);
  auto constexpr screwRadius = screwDiameter / 2.F;
  auto const panelWidth = hp2mm((float)hp);

  auto constexpr top = screwRadius;
  auto constexpr bottom = moduleHeight - screwRadius;

  auto const left = hp < 3 ? screwRadius : screwRadius + screwDiameter;
  auto const right = panelWidth - left;

  auto screwPositions =
      std::vector<rack::math::Vec>{mm2px(left, top), mm2px(right, bottom)};
  if (hp > 4) {
    screwPositions.push_back(mm2px(right, top));
    screwPositions.push_back(mm2px(left, bottom));
  }

  std::shuffle(screwPositions.begin(), screwPositions.end(),
               std::mt19937(std::random_device()()));

  auto const positionOfSpecialScrew = screwPositions.back();
  panel->addChild(
      rack::createWidgetCentered<SpecialScrew>(positionOfSpecialScrew));

  screwPositions.pop_back();

  for (auto const screwPosition : screwPositions) {
    panel->addChild(rack::createWidgetCentered<NormalScrew>(screwPosition));
  }
}

} // namespace dhe
