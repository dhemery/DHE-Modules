#include <algorithm>
#include <util/signal.hpp>

#include "dhe-modules.hpp"
#include "module-widget.hpp"
#include "module.hpp"

#include "util/range.hpp"

namespace DHE {

struct RangerLevel {
  rack::Param const &knob;
  rack::Input const &cv;
  rack::Param const &av;

  RangerLevel(rack::Param const &knob, rack::Input const &cv, rack::Param const &av)
      : knob{knob}, cv{cv}, av{av} {}

  auto operator()(float limit1, float limit2) const -> float {
    auto const range{Range{limit1, limit2}};
    auto const input{Module::modulated(knob, cv, av)};
    return range.scale(input);
  }
};

struct RangerLimit {
  rack::Param const &knob;
  rack::Input const &cv;
  rack::Param const &av;
  rack::Param const &range_selector;

  RangerLimit(rack::Param const &knob, rack::Input const &cv, rack::Param const &av, rack::Param const &range_selector)
      : knob{knob}, cv{cv}, av{av}, range_selector{range_selector} {}

  auto operator()() const -> float {
    auto const range{range_selector.value > 0.1 ? Signal::unipolar_range : Signal::bipolar_range};
    auto const input{Module::modulated(knob, cv, av)};
    return range.scale(input);
  }
};

struct Ranger : Module {
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
  enum OutputIds { OUT, OUTPUT_COUNT };

  RangerLevel level{params[LEVEL_KNOB], inputs[LEVEL_CV], params[LEVEL_AV]};
  RangerLimit upper{params[LIMIT_1_KNOB], inputs[LIMIT_1_CV], params[LIMIT_1_AV], params[LIMIT_1_RANGE]};
  RangerLimit lower{params[LIMIT_2_KNOB], inputs[LIMIT_2_CV], params[LIMIT_2_AV], params[LIMIT_2_RANGE]};

  Ranger() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  void step() override {
    outputs[OUT].value = level(lower(), upper());
  }
};

struct RangerWidget : public ModuleWidget {
  explicit RangerWidget(rack::Module *module)
      : ModuleWidget(module, 6, "ranger") {
    auto widget_right_edge = width();

    auto left_x = width()/3.5f + 0.333333333f;
    auto right_x = widget_right_edge - left_x;

    auto y = 24.f;
    auto delta_y = 16.f;
    auto panel_buffer = 4.f;

    install_knob("medium", Ranger::LEVEL_KNOB, {left_x, y});
    install_output(Ranger::OUT, {right_x, y});
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

rack::Model
    *
    modelRanger = rack::Model::create<DHE::Ranger, DHE::RangerWidget>(
    "DHE-Modules", "Ranger", "Ranger", rack::UTILITY_TAG, rack::WAVESHAPER_TAG);
