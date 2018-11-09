#include <engine.hpp>

#include "dhe-modules.hpp"
#include "module-widget.hpp"
#include "module.hpp"

#include "util/controls.hpp"
#include "util/d-flip-flop.hpp"
#include "util/mode.hpp"
#include "util/ramp.hpp"
#include "util/range.hpp"

namespace DHE {
struct Stage : public Module {
  std::function<float()> level_knob = knob(LEVEL_KNOB);
  Mode stage_mode = {};
  Mode defer_mode = {};

  // TODO: Move this inside stage mode or an envelope class.
  float phase_0_voltage{0.f};

  Ramp envelope = Ramp{[this] { return sample_time() / duration_in(); }};
  DFlipFlop envelope_trigger = DFlipFlop{[this] { return envelope_gate_in(); }};
  Ramp eoc_pulse = Ramp{[this] { return sample_time() / 1e-3f; }};

  SubmodeSwitch executor = {[this] { return defer_gate_in(); }, &stage_mode,
                            &defer_mode};

  void onReset() override { Module::onReset(); }

  Stage() : Module(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT) {
    defer_mode.on_entry([this] { send_active(true); });
    defer_mode.on_step([this] { send_envelope(envelope_in()); });

    stage_mode.on_entry([this] {
      send_active(false);
      phase_0_voltage = envelope_in();
      envelope_trigger.enable();
    });
    stage_mode.on_step([this] {
      envelope_trigger.step();
      envelope.step();
      send_envelope(envelope_voltage(
          envelope.phase())); // TODO: Move to envelope.on_step()
    });
    stage_mode.on_exit([this] {
      envelope_trigger.disable();
      envelope.stop();
    });

    envelope_trigger.on_rise([this] {
      phase_0_voltage = envelope_in();
      envelope.start();
    });

    envelope.on_start([this] { send_active(true); });
    envelope.on_completion([this] {
      send_active(false);
      eoc_pulse.start();
    });

    eoc_pulse.on_start([this] { send_eoc(true); });
    eoc_pulse.on_completion([this] { send_eoc(false); });

    executor.on_step([this] { eoc_pulse.step(); });
    executor.enter();
  }

  float defer_gate_in() const { return input(DEFER_IN); }

  float duration_in() const {
    return Duration::scaled(param(DURATION_KNOB), Duration::MEDIUM_RANGE);
  }

  float envelope_gate_in() const { return input(TRIGGER_IN); }

  float envelope_in() const { return input(ENVELOPE_IN); }

  float envelope_voltage(float phase) const {
    return scale(taper(phase), phase_0_voltage, level_in());
  }

  float level_in() const {
    static constexpr auto level_range = Range{0.f, 10.f};
    return level_range.scale(level_knob());
   }

  float sample_time() const { return rack::engineGetSampleTime(); }

  void send_active(bool is_active) {
    outputs[ACTIVE_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(is_active);
  }

  void send_envelope(float voltage) { outputs[ENVELOPE_OUT].value = voltage; }

  void send_eoc(bool is_pulsing) {
    outputs[EOC_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(is_pulsing);
  }

  void step() override { executor.step(); }

  float taper(float phase) const { return Taper::j(phase, param(CURVE_KNOB)); }

  enum ParameterIIds { DURATION_KNOB, LEVEL_KNOB, CURVE_KNOB, PARAMETER_COUNT };

  enum InputIds { ENVELOPE_IN, TRIGGER_IN, DEFER_IN, INPUT_COUNT };

  enum OutputIds { ENVELOPE_OUT, EOC_OUT, ACTIVE_OUT, OUTPUT_COUNT };
};

struct StageWidget : public ModuleWidget {
  StageWidget(rack::Module *module) : ModuleWidget(module, 5, "stage") {
    auto widget_right_edge = width();

    auto left_x = width() / 4.f + 0.333333f;
    auto center_x = widget_right_edge / 2.f;
    auto right_x = widget_right_edge - left_x;

    auto top_row_y = 25.f;
    auto row_spacing = 18.5f;

    auto row = 0;
    install_knob("large", Stage::LEVEL_KNOB,
                 {center_x, top_row_y + row * row_spacing});

    row++;
    install_knob("large", Stage::CURVE_KNOB,
                 {center_x, top_row_y + row * row_spacing});

    row++;
    install_knob("large", Stage::DURATION_KNOB,
                 {center_x, top_row_y + row * row_spacing});

    top_row_y = 82.f;
    row_spacing = 15.f;

    row = 0;
    install_input(Stage::DEFER_IN, {left_x, top_row_y + row * row_spacing});
    install_output(Stage::ACTIVE_OUT, {right_x, top_row_y + row * row_spacing});

    row++;
    install_input(Stage::TRIGGER_IN, {left_x, top_row_y + row * row_spacing});
    install_output(Stage::EOC_OUT, {right_x, top_row_y + row * row_spacing});

    row++;
    install_input(Stage::ENVELOPE_IN, {left_x, top_row_y + row * row_spacing});
    install_output(Stage::ENVELOPE_OUT,
                   {right_x, top_row_y + row * row_spacing});
  }
};
} // namespace DHE
rack::Model *modelStage = rack::Model::create<DHE::Stage, DHE::StageWidget>(
    "DHE-Modules", "Stage", "Stage", rack::ENVELOPE_GENERATOR_TAG);
