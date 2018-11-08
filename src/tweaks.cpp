#include "dhe-modules.hpp"
#include "module-widget.hpp"
#include "module.hpp"

#include "util/controls.hpp"

namespace DHE {

struct Tweaks : Module {

  Tweaks() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  void step() override {}

  enum ParameterIds {
    KNOB_1,
    KNOB_2,
    KNOB_3,
    KNOB_4,
    KNOB_5,
    MODE_1,
    MODE_2,
    MODE_3,
    MODE_4,
    MODE_5,
    PARAMETER_COUNT
  };
  enum InputIds { IN_1, IN_2, IN_3, IN_4, IN_5, INPUT_COUNT };
  enum OutputIds { OUT_1, OUT_2, OUT_3, OUT_4, OUT_5, OUTPUT_COUNT };
};

struct TweaksWidget : public ModuleWidget {
  TweaksWidget(rack::Module *module) : ModuleWidget(module, 11, "tweaks") {
    auto widget_right_edge = width();

    auto left_x = widget_right_edge / 7.f;
    auto right_x = widget_right_edge - left_x;
    auto left_center_x = (right_x - left_x) / 3.f + left_x;
    auto right_center_x = widget_right_edge - left_center_x;

    auto top_row_y = 27.f;
    auto row_spacing = 20.f;

    for (int row = 0; row < 5; row++) {
      install_input(Tweaks::IN_1 + row, {left_x, top_row_y + row * row_spacing});
      install_knob(
          "large", Tweaks::KNOB_1 + row,
          {left_center_x + 1.25f, top_row_y - 1.25f + row * row_spacing});
      install_switch(Tweaks::MODE_1 + row,
                     {right_center_x, top_row_y - 1.25f + row * row_spacing}, 2,
                     2);
      install_output(Tweaks::OUT_1 + row, {right_x, top_row_y + row * row_spacing});
    }
  }
};

} // namespace DHE
rack::Model *modelTweaks = rack::Model::create<DHE::Tweaks, DHE::TweaksWidget>(
    "DHE-Modules", "Tweaks", "Tweaks", rack::UTILITY_TAG);
