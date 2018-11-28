#include <utility>

#include <utility>

#include "dhe-modules.h"
#include "module-widget.h"

#include "controls/knob.h"
#include "controls/signal.h"
#include "util/range.h"
#include <controls/switch.h>

namespace DHE {

struct Ranger : rack::Module {
  enum ParameterIds {
    LEVEL_KNOB,
    LEVEL_AV,
    LIMIT_1_KNOB,
    LIMIT_1_AV,
    LIMIT_1_RANGE,
    LIMIT_2_KNOB,
    LIMIT_2_AV,
    LIMIT_2_RANGE,
    PARAMETER_COUNT
  };
  enum InputIds { LEVEL_CV, LIMIT_1_CV, LIMIT_2_CV, INPUT_COUNT };
  enum OutputIds { MAIN_OUT, OUTPUT_COUNT };

  Knob upper = Knob{this, LIMIT_1_KNOB,LIMIT_1_CV, LIMIT_1_AV}
                   .scale_to(Signal::range_switch(this, LIMIT_1_RANGE));
  Knob lower = Knob{this, LIMIT_2_KNOB, LIMIT_2_CV, LIMIT_2_AV}
                   .scale_to(Signal::range_switch(this, LIMIT_2_RANGE));
  Knob level =
      Knob{this, LEVEL_KNOB, LEVEL_CV, LEVEL_AV}.scale_to([this] {
        return Range{lower(), upper()};
      });

  Ranger() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  void send_main_out(float voltage) { outputs[MAIN_OUT].value = voltage; }

  void step() override { send_main_out(level()); }
};

struct RangerWidget : public ModuleWidget {
  explicit RangerWidget(rack::Module *module)
      : ModuleWidget(module, 6, "ranger") {
    auto widget_right_edge = width();

    auto left_x = width() / 3.5f + 0.333333333f;
    auto right_x = widget_right_edge - left_x;

    auto y = 24.f;
    auto delta_y = 16.f;
    auto panel_buffer = 4.f;

    install_knob("medium", Ranger::LEVEL_KNOB, {left_x, y});
    install_output(Ranger::MAIN_OUT, {right_x, y});
    y += delta_y;
    install_input(Ranger::LEVEL_CV, {left_x, y});
    install_knob("tiny", Ranger::LEVEL_AV, {right_x, y});

    y += delta_y + panel_buffer;

    install_knob("medium", Ranger::LIMIT_1_KNOB, {left_x, y});
    install_switch(Ranger::LIMIT_1_RANGE, {right_x, y});
    y += delta_y;
    install_input(Ranger::LIMIT_1_CV, {left_x, y});
    install_knob("tiny", Ranger::LIMIT_1_AV, {right_x, y});

    y += delta_y + panel_buffer;

    install_knob("medium", Ranger::LIMIT_2_KNOB, {left_x, y});
    install_switch(Ranger::LIMIT_2_RANGE, {right_x, y});
    y += delta_y;
    install_input(Ranger::LIMIT_2_CV, {left_x, y});
    install_knob("tiny", Ranger::LIMIT_2_AV, {right_x, y});
  }
};
} // namespace DHE

rack::Model *modelRanger = rack::Model::create<DHE::Ranger, DHE::RangerWidget>(
    "DHE-Modules", "Ranger", "Ranger", rack::UTILITY_TAG, rack::WAVESHAPER_TAG);
