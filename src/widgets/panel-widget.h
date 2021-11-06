#pragma once

#include "rack.hpp"

namespace dhe {
template <typename TPanel> struct PanelWidget : public rack::app::ModuleWidget {
  PanelWidget(rack::engine::Module *module) {
    setModule(module);
    setPanel(load_svg(TPanel::svg_dir, TPanel::panel_file));
    install_screws();
  }

  using NormalScrew = rack::componentlibrary::ScrewSilver;
  using SpecialScrew = rack::componentlibrary::ScrewBlack;

  void install_screws() {
    auto constexpr screw_diameter = hp2mm(1.F);
    auto constexpr screw_radius = screw_diameter / 2.F;
    auto const panel_width = hp2mm((float)TPanel::hp);

    auto constexpr top = screw_radius;
    auto constexpr bottom = module_height - screw_radius;

    auto const left =
        TPanel::hp < 3 ? screw_radius : screw_radius + screw_diameter;
    auto const right = panel_width - left;

    auto screw_positions =
        std::vector<rack::math::Vec>{mm2px(left, top), mm2px(right, bottom)};
    if (TPanel::hp > 4) {
      screw_positions.push_back(mm2px(right, top));
      screw_positions.push_back(mm2px(left, bottom));
    }

    std::shuffle(screw_positions.begin(), screw_positions.end(),
                 std::mt19937(std::random_device()()));

    auto const position_of_special_screw = screw_positions.back();
    addChild(
        rack::createWidgetCentered<SpecialScrew>(position_of_special_screw));

    screw_positions.pop_back();

    for (auto const &screw_position : screw_positions) {
      addChild(rack::createWidgetCentered<NormalScrew>(screw_position));
    }
  }
};

} // namespace dhe
