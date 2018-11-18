#include <algorithm>

#include "dhe-modules.hpp"
#include "module-widget.hpp"
#include "module.hpp"

#include "util/range.hpp"

namespace DHE {

struct Ranger : Module {
  const std::function<float()> level = knob(LEVEL_KNOB, LEVEL_CV_IN, LEVEL_AV_KNOB);
  const std::function<float()> limit1 =
      knob(LIMIT_1_KNOB, LIMIT_1_CV_IN, LIMIT_1_AV_KNOB);
  const std::function<float()> limit2 =
      knob(LIMIT_2_KNOB, LIMIT_2_CV_IN, LIMIT_2_AV_KNOB);
  const std::function<const Range &()> range1 = range_switch(LIMIT_1_RANGE_SWITCH);
  const std::function<const Range &()> range2 = range_switch(LIMIT_2_RANGE_SWITCH);

  Ranger() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  void step() override {
    auto cw_limit = range1().scale(limit1());
    auto ccw_limit = range2().scale(limit2());

    auto range = Range{ccw_limit, cw_limit};
    auto out = range.scale(level());

    outputs[OUT].value = out;
  }

  enum ParameterIds {
    LEVEL_KNOB,
    LEVEL_AV_KNOB,
    LIMIT_1_KNOB,
    LIMIT_1_AV_KNOB,
    LIMIT_1_RANGE_SWITCH,
    LIMIT_2_KNOB,
    LIMIT_2_AV_KNOB,
    LIMIT_2_RANGE_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds { LEVEL_CV_IN, LIMIT_1_CV_IN, LIMIT_2_CV_IN, INPUT_COUNT };
  enum OutputIds { OUT, OUTPUT_COUNT };
};

struct RangerWidget : public ModuleWidget {
  explicit RangerWidget(rack::Module *module) : ModuleWidget(module, 6, "ranger") {
    auto widget_right_edge = width();

    auto left_x = width() / 3.5f + 0.333333333f;
    auto right_x = widget_right_edge - left_x;

    auto y = 24.f;
    auto delta_y = 16.f;
    auto panel_buffer = 4.f;

    install_knob("medium", Ranger::LEVEL_KNOB, {left_x, y});
    install_output(Ranger::OUT, {right_x, y});
    y += delta_y;
    install_input(Ranger::LEVEL_CV_IN, {left_x, y});
    install_knob("tiny", Ranger::LEVEL_AV_KNOB, {right_x, y});

    y += delta_y + panel_buffer;

    install_knob("medium", Ranger::LIMIT_1_KNOB, {left_x, y});
    install_switch(Ranger::LIMIT_1_RANGE_SWITCH, {right_x, y});
    y += delta_y;
    install_input(Ranger::LIMIT_1_CV_IN, {left_x, y});
    install_knob("tiny", Ranger::LIMIT_1_AV_KNOB, {right_x, y});

    y += delta_y + panel_buffer;

    install_knob("medium", Ranger::LIMIT_2_KNOB, {left_x, y});
    install_switch(Ranger::LIMIT_2_RANGE_SWITCH, {right_x, y});
    y += delta_y;
    install_input(Ranger::LIMIT_2_CV_IN, {left_x, y});
    install_knob("tiny", Ranger::LIMIT_2_AV_KNOB, {right_x, y});
  }
};
} // namespace DHE

rack::Model *modelRanger = rack::Model::create<DHE::Ranger, DHE::RangerWidget>(
    "DHE-Modules", "Ranger", "Ranger", rack::UTILITY_TAG, rack::WAVESHAPER_TAG);
