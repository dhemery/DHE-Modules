#include <engine.hpp>

#include "dhe-modules.hpp"
#include "module-widget.hpp"
#include "module.hpp"

#include "util/d-flip-flop.hpp"
#include "util/duration.hpp"
#include "util/mode.hpp"
#include "util/phase-accumulator.hpp"
#include "util/range.hpp"
#include "util/taper.hpp"

namespace DHE {
struct Stage : public Module {
  std::function<float()> const level_knob{knob(LEVEL_KNOB)};
  std::function<float()> const duration_knob{knob(DURATION_KNOB)};
  std::function<float()> const duration{Duration::of(duration_knob)};
  std::function<float()> const curve_knob{knob(CURVE_KNOB)};
  std::function<float()> const defer_gate_in{float_in(DEFER_IN)};
  std::function<bool()> const envelope_gate_in{bool_in(TRIGGER_IN)};
  std::function<float()> const envelope_in{float_in(ENVELOPE_IN)};

  Mode stage_mode{};
  Mode defer_mode{};

  // TODO: Move this inside stage mode or an envelope class.
  float phase_0_voltage{0.f};

  PhaseAccumulator envelope{[this] { return sample_time() / duration(); }};
  DFlipFlop envelope_trigger{envelope_gate_in};
  PhaseAccumulator eoc_pulse{[this] { return sample_time() / 1e-3f; }};

  SubmodeSwitch executor{defer_gate_in, &stage_mode, &defer_mode};

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

  auto envelope_voltage(float phase) const -> float {
    return scale(taper(phase), phase_0_voltage, level_in());
  }

  auto level_in() const -> float {
    static constexpr auto level_range = Range{0.f, 10.f};
    return level_range.scale(level_knob());
  }

  auto sample_time() const -> float { return rack::engineGetSampleTime(); }

  void send_active(bool is_active) {
    outputs[ACTIVE_OUT].value = is_active ? 10.f : 0.f;
  }

  void send_envelope(float voltage) { outputs[ENVELOPE_OUT].value = voltage; }

  void send_eoc(bool is_pulsing) {
    outputs[EOC_OUT].value = is_pulsing ? 10.f : 0.f;
  }

  void step() override { executor.step(); }

  auto taper(float phase) const -> float {
    return Taper::j(phase, curve_knob());
  }

  enum ParameterIIds { DURATION_KNOB, LEVEL_KNOB, CURVE_KNOB, PARAMETER_COUNT };

  enum InputIds { ENVELOPE_IN, TRIGGER_IN, DEFER_IN, INPUT_COUNT };

  enum OutputIds { ENVELOPE_OUT, EOC_OUT, ACTIVE_OUT, OUTPUT_COUNT };
};

struct StageWidget : public ModuleWidget {
  explicit StageWidget(rack::Module *module)
      : ModuleWidget(module, 5, "stage") {
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
