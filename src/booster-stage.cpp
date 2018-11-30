#include "dhe-modules.h"
#include "module-widget.h"

#include "util/d-flip-flop.h"
#include "util/duration.h"
#include "util/knob.h"
#include "util/mode.h"
#include "util/phase-accumulator.h"
#include "util/signal.h"

namespace DHE {

class BoosterStage : public rack::Module {
public:
  BoosterStage() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
    defer_mode.on_entry([this] { is_active = true; });
    defer_mode.on_step([this] { send_envelope(envelope_in()); });

    stage_mode.on_entry([this] {
      is_active = false;
      phase_0_voltage = envelope_in();
      envelope_trigger.enable();
    });
    stage_mode.on_step([this] {
      envelope_trigger.step();
      envelope.step();
      send_envelope(envelope_voltage(envelope.phase()));
    });
    stage_mode.on_exit([this] {
      envelope_trigger.disable();
      envelope.stop();
    });

    envelope_trigger.on_rise([this] {
      phase_0_voltage = envelope_in();
      envelope.start();
    });

    envelope.on_start([this] { is_active = true; });
    envelope.on_completion([this] {
      is_active = false;
      eoc_pulse.start();
    });

    eoc_pulse.on_start([this] { is_eoc = true; });
    eoc_pulse.on_completion([this] { is_eoc = false; });

    executor.on_step([this] { eoc_pulse.step(); });
    executor.enter();
  }

  void step() override {
    executor.step();
    send_active();
    send_eoc();
  }

  enum ParameterIds {
    ACTIVE_BUTTON,
    CURVE_KNOB,
    DEFER_BUTTON,
    DURATION_KNOB,
    DURATION_SWITCH,
    EOC_BUTTON,
    LEVEL_KNOB,
    LEVEL_SWITCH,
    SHAPE_SWITCH,
    TRIGGER_BUTTON,
    PARAMETER_COUNT
  };

  enum InputIds {
    CURVE_CV,
    DEFER_IN,
    DURATION_CV,
    LEVEL_CV,
    MAIN_IN,
    TRIGGER_IN,
    INPUT_COUNT
  };

  enum OutputIds { ACTIVE_OUT, EOC_OUT, MAIN_OUT, OUTPUT_COUNT };

private:
  auto curvature() const -> float {
    return Sigmoid::curvature(modulated(CURVE_KNOB, CURVE_CV));
  }

  auto defer_in() const -> bool {
    auto defer_input = inputs[DEFER_IN].value > 0.1f;
    auto defer_button = params[DEFER_BUTTON].value > 0.1f;
    return defer_button || defer_input;
  }

  auto duration() const -> float {
    auto rotation = modulated(DURATION_KNOB, DURATION_CV);
    auto selection = params[DURATION_SWITCH].value;
    return DHE::duration(rotation, selection);
  }

  auto envelope_in() const -> float { return inputs[MAIN_IN].value; }

  auto envelope_voltage(float phase) const -> float {
    auto tapered = Sigmoid::taper(phase, curvature(), is_s_shape());
    return scale(tapered, phase_0_voltage, level());
  }

  auto is_s_shape() const -> bool { return params[SHAPE_SWITCH].value > 0.5f; }

  auto level() const -> float {
    auto is_uni = params[LEVEL_SWITCH].value > 0.5f;
    auto range = Signal::range(is_uni);
    auto level = modulated(LEVEL_KNOB, LEVEL_CV);
    return range.scale(level);
  }

  auto modulated(ParameterIds knob_param, InputIds cv_input) const -> float {
    auto rotation = params[knob_param].value;
    auto cv = inputs[cv_input].value;
    return Knob::modulated(rotation, cv);
  }

  auto sample_time() const -> float { return rack::engineGetSampleTime(); }

  void send_active() {
    auto active_button = params[ACTIVE_BUTTON].value > 0.1f;
    auto active = is_active || active_button;
    outputs[ACTIVE_OUT].value = active ? 10.f : 0.f;
  }

  void send_eoc() {
    auto eoc_button = params[EOC_BUTTON].value > 0.1f;
    auto eoc = is_eoc || eoc_button;
    outputs[EOC_OUT].value = eoc ? 10.f : 0.f;
  }

  void send_envelope(float voltage) { outputs[MAIN_OUT].value = voltage; }

  auto trigger_in() const -> bool {
    auto trigger_button = params[TRIGGER_BUTTON].value > 0.1;
    auto trigger_input = inputs[TRIGGER_IN].value > 0.1;
    return trigger_button || trigger_input;
  }

  float phase_0_voltage = 0.f;
  bool is_active = false;
  bool is_eoc = false;

  PhaseAccumulator envelope{[this] { return sample_time() / duration(); }};
  PhaseAccumulator eoc_pulse{[this] { return sample_time() / 1e-3f; }};

  DFlipFlop envelope_trigger{[this] { return trigger_in(); }};

  Mode stage_mode{};

  Mode defer_mode{};

  CompoundMode executor{[this] { return defer_in(); },
                        {&stage_mode, &defer_mode}};
};

struct BoosterStageWidget : public ModuleWidget {
  explicit BoosterStageWidget(rack::Module *module)
      : ModuleWidget(module, 8, "booster-stage") {
    auto widget_right_edge = width();

    auto left_x = widget_right_edge / 6.f + 0.3333333f;
    auto center_x = widget_right_edge / 2.f;
    auto right_x = widget_right_edge - left_x;
    auto button_port_distance = 7.891f;
    auto center_left_x = left_x + button_port_distance;
    auto center_right_x = right_x - button_port_distance;

    auto top_row_y = 25.f;
    auto row_spacing = 18.5f;

    auto row = 0;
    install_input(BoosterStage::LEVEL_CV,
                  {left_x, top_row_y + row * row_spacing});
    install_knob("large", BoosterStage::LEVEL_KNOB,
                 {center_x, top_row_y + row * row_spacing});
    install_switch(BoosterStage::LEVEL_SWITCH,
                   {right_x, top_row_y + row * row_spacing}, 1, 1);

    row++;
    install_input(BoosterStage::CURVE_CV,
                  {left_x, top_row_y + row * row_spacing});
    install_knob("large", BoosterStage::CURVE_KNOB,
                 {center_x, top_row_y + row * row_spacing});
    install_switch(BoosterStage::SHAPE_SWITCH,
                   {right_x, top_row_y + row * row_spacing});

    row++;
    install_input(BoosterStage::DURATION_CV,
                  {left_x, top_row_y + row * row_spacing});
    install_knob("large", BoosterStage::DURATION_KNOB,
                 {center_x, top_row_y + row * row_spacing});
    install_switch(BoosterStage::DURATION_SWITCH,
                   {right_x, top_row_y + row * row_spacing}, 2, 1);

    top_row_y = 82.f;
    row_spacing = 15.f;

    row = 0;
    install_input(BoosterStage::DEFER_IN,
                  {left_x, top_row_y + row * row_spacing});
    install_button("normal", BoosterStage::DEFER_BUTTON,
                   {center_left_x, top_row_y + row * row_spacing});
    install_button("reverse", BoosterStage::ACTIVE_BUTTON,
                   {center_right_x, top_row_y + row * row_spacing});
    install_output(BoosterStage::ACTIVE_OUT,
                   {right_x, top_row_y + row * row_spacing});

    row++;
    install_input(BoosterStage::TRIGGER_IN,
                  {left_x, top_row_y + row * row_spacing});
    install_button("normal", BoosterStage::TRIGGER_BUTTON,
                   {center_left_x, top_row_y + row * row_spacing});
    install_button("reverse", BoosterStage::EOC_BUTTON,
                   {center_right_x, top_row_y + row * row_spacing});
    install_output(BoosterStage::EOC_OUT,
                   {right_x, top_row_y + row * row_spacing});

    row++;
    install_input(BoosterStage::MAIN_IN,
                  {left_x, top_row_y + row * row_spacing});
    install_output(BoosterStage::MAIN_OUT,
                   {right_x, top_row_y + row * row_spacing});
  }
};
} // namespace DHE
rack::Model *modelBoosterStage =
    rack::Model::create<DHE::BoosterStage, DHE::BoosterStageWidget>(
        "DHE-Modules", "BoosterStage", "Booster Stage",
        rack::ENVELOPE_GENERATOR_TAG);
