#include <engine.hpp>

#include "dhe-modules.hpp"
#include "module-widget.hpp"
#include "module.hpp"

#include "util/d-flip-flop.hpp"
#include "util/duration.hpp"
#include "util/mode.hpp"
#include "util/phase-accumulator.hpp"
#include "util/range.hpp"
#include "util/sigmoid.hpp"
#include "util/signal.hpp"

namespace DHE {

struct Stage : public Module {
  enum ParameterIIds { DURATION_KNOB, LEVEL_KNOB, CURVE_KNOB, PARAMETER_COUNT };

  enum InputIds { ENVELOPE_IN, TRIGGER_IN, DEFER_IN, INPUT_COUNT };

  enum OutputIds { ENVELOPE_OUT, EOC_OUT, ACTIVE_OUT, OUTPUT_COUNT };

  float phase_0_voltage{0.f};

  PhaseAccumulator generator{[this] { return sample_time() / duration_in(); }};
  DFlipFlop envelope_trigger{[this]() -> bool { return trigger_in(); }};
  PhaseAccumulator eoc_pulse{[this] { return sample_time() / 1e-3f; }};

  Mode generating_mode{};
  Mode deferring_mode{};
  std::vector<Mode *> const modes{&generating_mode, &deferring_mode};
  CompoundMode executor{[this]() -> bool { return defer_in(); }, modes};

  Stage() : Module(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT) {
    deferring_mode.on_entry([this] { send_active(true); });
    deferring_mode.on_step([this] { send_envelope(envelope_in()); });

    generating_mode.on_entry([this] {
      send_active(false);
      phase_0_voltage = envelope_in();
      envelope_trigger.enable();
    });
    generating_mode.on_step([this] {
      envelope_trigger.step();
      generator.step();
      send_envelope(envelope_voltage(generator.phase()));
    });
    generating_mode.on_exit([this] {
      envelope_trigger.disable();
      generator.stop();
    });

    envelope_trigger.on_rise([this] {
      phase_0_voltage = envelope_in();
      generator.start();
    });

    generator.on_start([this] { send_active(true); });
    generator.on_completion([this] {
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

  auto sample_time() const -> float { return rack::engineGetSampleTime(); }

  void step() override { executor.step(); }

  auto taper(float phase) const -> float {
    float curviness = curve_in();
    return j_taper(phase, curvature(curviness));
  }

  auto curve_in() const -> float {
    return params[CURVE_KNOB].value;
  }

  auto defer_in() const -> bool {
    return inputs[DEFER_IN].value > 0.1;
  }

  auto duration_in() const -> float {
    auto rotation{params[DURATION_KNOB].value};
    return Duration::of(rotation);
  }

  auto envelope_in() const -> float {
    return inputs[ENVELOPE_IN].value;
  }

  auto level_in() const -> float {
    auto rotation{params[LEVEL_KNOB].value};
    return Signal::unipolar_range.scale(rotation);
  }

  void send_active(bool is_active) {
    outputs[ACTIVE_OUT].value = is_active ? 10.f : 0.f;
  }

  void send_envelope(float envelope) {
    outputs[ENVELOPE_OUT].value = envelope;
  }

  void send_eoc(bool eoc) {
    outputs[EOC_OUT].value = eoc ? 10.f : 0.f;
  }

  auto trigger_in() const -> bool {
    return inputs[TRIGGER_IN].value > 0.1;
  }
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
