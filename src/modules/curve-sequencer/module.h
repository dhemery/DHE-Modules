#pragma once

#include "components/phase-timer.h"
#include "config/common-config.h"
#include "config/curvature-config.h"
#include "config/duration-config.h"
#include "config/level-config.h"
#include "controls.h"
#include "generator.h"
#include "sequence-controller.h"
#include "step-controller.h"
#include "step-selector.h"

#include <engine/Module.hpp>

namespace dhe {

namespace curve_sequencer {
template <int N> class Module : public rack::engine::Module {
  using ControlsT = Controls<rack::engine::Input, rack::engine::Output,
                             rack::engine::Param, rack::engine::Light, N>;

public:
  Module() {
    config(enum_index(ControlsT::Param::Count),
           enum_index(ControlsT::Input::Count),
           enum_index(ControlsT::Output::Count),
           enum_index(ControlsT::Light::Count));

    config_button(this, enum_index(ControlsT::Param::Run), "Run",
                  {"from input", "Yes"}, 1);
    config_button(this, enum_index(ControlsT::Param::Gate), "Gate",
                  {"from input", "High"}, 0);
    config_button(this, enum_index(ControlsT::Param::Loop), "Loop",
                  {"from input", "Yes"}, 0);
    config_button(this, enum_index(ControlsT::Param::Reset), "Reset",
                  {"from input", "High"}, 0);

    configParam(enum_index(ControlsT::Param::SelectionStart), 0.F, N - 1, 0.F,
                "Start step", "", 0.F, 1.F, 1.F);
    configParam(enum_index(ControlsT::Param::SelectionLength), 1.F, N, N,
                "Sequence length", " steps");

    config_level_range_switch(this, enum_index(ControlsT::Param::LevelRange));
    config_duration_range_switch(this,
                                 enum_index(ControlsT::Param::DurationRange));

    for (auto step = 0; step < N; step++) {
      config_level_knob(this, enum_index(ControlsT::Param::StepEndLevel, step),
                        enum_index(ControlsT::Param::LevelRange), "Level");
      config_curve_shape_switch(
          this, enum_index(ControlsT::Param::StepShape, step), "Shape");
      config_curvature_knob(
          this, enum_index(ControlsT::Param::StepCurvature, step), "Curvature");
      config_duration_knob(
          this, enum_index(ControlsT::Param::StepDuration, step),
          enum_index(ControlsT::Param::DurationRange), "Duration");
      config_button(this, enum_index(ControlsT::Param::EnableStep, step),
                    "Enabled", {"from input", "Yes"}, 1);

      controls_.show_inactive(step);
    }
  }

  ~Module() override = default;

  void process(ProcessArgs const &args) override {
    curve_sequencer_.execute(args.sampleTime);
  }

private:
  PhaseTimer timer_{};
  ControlsT controls_{inputs, outputs, params, lights};
  SourceController<ControlsT> start_source_{controls_};
  SourceController<ControlsT> end_source_{controls_};
  Interrupter<ControlsT> interrupter_{controls_};
  Generator<ControlsT, decltype(start_source_)> generator_{
      controls_, start_source_, end_source_};
  Sustainer<ControlsT> sustainer_{controls_};
  StepController<decltype(interrupter_), decltype(generator_),
                 decltype(sustainer_)>
      step_controller_{interrupter_, generator_, sustainer_};
  StepSelector<ControlsT> selector_{controls_, N};

  SequenceController<ControlsT, decltype(selector_), decltype(step_controller_)>
      curve_sequencer_{controls_, selector_, step_controller_};
};
} // namespace curve_sequencer

} // namespace dhe
