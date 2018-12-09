#include "dhe-modules.h"
#include "module-widget.h"

#include "util/knob.h"
#include "util/range.h"
#include "util/signal.h"

namespace DHE {

class Ranger : public rack::Module {
public:
  Ranger() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  void step() override { send_main_out(scale(level(), limit2(), limit1())); }

  enum ParameterIds {
    LEVEL_KNOB,
    LEVEL_AV,
    LIMIT_1_KNOB,
    LIMIT_1_AV,
    LIMIT_1_RANGE_SWITCH,
    LIMIT_2_KNOB,
    LIMIT_2_AV,
    LIMIT_2_RANGE_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds { LEVEL_CV, LIMIT_1_CV, LIMIT_2_CV, INPUT_COUNT };
  enum OutputIds { MAIN_OUT, OUTPUT_COUNT };

private:
  auto level() const -> float {
    return modulated(LEVEL_KNOB, LEVEL_CV, LEVEL_AV);
  }

  auto limit(ParameterIds knob_param, InputIds cv_input, ParameterIds av_param,
             int range_switch_param) const -> float {
    auto is_uni = params[range_switch_param].value > 0.5f;
    auto range = Signal::range(is_uni);
    return range.scale(modulated(knob_param, cv_input, av_param));
  }

  auto limit1() const -> float {
    return limit(LIMIT_1_KNOB, LIMIT_1_CV, LIMIT_1_AV, LIMIT_1_RANGE_SWITCH);
  }

  auto limit2() const -> float {
    return limit(LIMIT_2_KNOB, LIMIT_2_CV, LIMIT_2_AV, LIMIT_2_RANGE_SWITCH);
  }

  auto modulated(ParameterIds knob_param, InputIds cv_input,
                 ParameterIds av_parm) const -> float {
    auto rotation = params[knob_param].value;
    auto cv = inputs[cv_input].value;
    auto av = params[av_parm].value;
    return Knob::modulated(rotation, cv, av);
  }

  void send_main_out(float voltage) { outputs[MAIN_OUT].value = voltage; }
};

struct RangerWidget : public ModuleWidget<RangerWidget, Ranger> {
  static constexpr auto resource_name = "ranger";

  explicit RangerWidget(Ranger *module)
      : ModuleWidget(module, 6) {
    auto widget_right_edge = width();

    auto left_x = width() / 3.5f + 0.333333333f;
    auto right_x = widget_right_edge - left_x;

    auto y = 24.f;
    auto delta_y = 16.f;
    auto panel_buffer = 4.f;

    install_medium_knob(Ranger::LEVEL_KNOB, {left_x, y});
    install_output(Ranger::MAIN_OUT, {right_x, y});
    y += delta_y;
    install_input(Ranger::LEVEL_CV, {left_x, y});
    install_tiny_knob(Ranger::LEVEL_AV, {right_x, y});

    y += delta_y + panel_buffer;

    install_medium_knob(Ranger::LIMIT_1_KNOB, {left_x, y});
    install_switch(Ranger::LIMIT_1_RANGE_SWITCH, {right_x, y});
    y += delta_y;
    install_input(Ranger::LIMIT_1_CV, {left_x, y});
    install_tiny_knob(Ranger::LIMIT_1_AV, {right_x, y});

    y += delta_y + panel_buffer;

    install_medium_knob(Ranger::LIMIT_2_KNOB, {left_x, y});
    install_switch(Ranger::LIMIT_2_RANGE_SWITCH, {right_x, y});
    y += delta_y;
    install_input(Ranger::LIMIT_2_CV, {left_x, y});
    install_tiny_knob(Ranger::LIMIT_2_AV, {right_x, y});
  }
};
} // namespace DHE

rack::Model *modelRanger = rack::Model::create<DHE::Ranger, DHE::RangerWidget>(
    "DHE-Modules", "Ranger", "Ranger", rack::UTILITY_TAG, rack::WAVESHAPER_TAG);
