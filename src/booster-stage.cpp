#include <utility>

#include "dhe-modules.hpp"
#include "module-widget.hpp"
#include "module.hpp"

#include "util/d-flip-flop.hpp"
#include "util/duration.hpp"
#include "util/mode.hpp"
#include "util/ramp.hpp"
#include "util/taper.hpp"

namespace DHE {

struct BoosterStage : Module {
  Mode stage_mode{};
  Mode defer_mode{};

  float phase_0_voltage{0.f};
  bool is_active{false};
  bool is_eoc{false};

  const std::function<float()> duration_knob = knob(DURATION_KNOB);
  const std::function<float()> level_knob = knob(LEVEL_KNOB, LEVEL_CV);
  const std::function<float()> curve_knob = knob(CURVE_KNOB, CURVE_CV);
  const std::function<const Range &()> level_range = range_switch(LEVEL_SWITCH);

  Ramp envelope{[this] { return sample_time() / duration_in(); }};
  Ramp eoc_pulse{[this] { return sample_time() / 1e-3f; }};
  DFlipFlop envelope_trigger{[this] { return envelope_gate_in(); }};

  SubmodeSwitch executor{[this] { return defer_gate_in(); }, &stage_mode,
                         &defer_mode};

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

  float defer_gate_in() const {
    return std::max(input(DEFER_IN), gate_button(DEFER_BUTTON));
  }

  float duration_in() const {
    const auto &range = Duration::range(param(DURATION_SWITCH));
    return Duration::scaled(duration_knob(), range);
  }

  float envelope_gate_in() const {
    return std::max(input(TRIGGER_IN), gate_button(TRIGGER_BUTTON));
  }

  float envelope_in() const { return input(ENVELOPE_IN); }

  float envelope_voltage(float phase) const {
    return scale(taper(phase), phase_0_voltage, level_in());
  }

  bool is_s_taper() const { return param(SHAPE_SWITCH) > 0.5; }

  float level_in() const { return level_range().scale(level_knob()); }

  void send_active() {
    auto active = is_active || param(ACTIVE_BUTTON) > 0.5f;
    outputs[ACTIVE_OUT].value = active ? 10.f : 0.f;
  }

  void send_envelope(float voltage) { outputs[ENVELOPE_OUT].value = voltage; }

  void send_eoc() {
    auto eoc = is_eoc || param(EOC_BUTTON) > 0.5f;
    outputs[EOC_OUT].value = eoc ? 10.f : 0.f;
  }

  float sample_time() const { return rack::engineGetSampleTime(); }

  float taper(float phase) const {
    auto curvature = curve_knob();
    return is_s_taper() ? Taper::s(phase, curvature)
                        : Taper::j(phase, curvature);
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
    ENVELOPE_IN,
    TRIGGER_IN,
    INPUT_COUNT
  };

  enum OutputIds { ACTIVE_OUT, EOC_OUT, ENVELOPE_OUT, OUTPUT_COUNT };
};

struct BoosterStageWidget : public ModuleWidget {
  BoosterStageWidget(rack::Module *module)
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
    install_input(BoosterStage::ENVELOPE_IN,
                  {left_x, top_row_y + row * row_spacing});
    install_output(BoosterStage::ENVELOPE_OUT,
                   {right_x, top_row_y + row * row_spacing});
  }
};
} // namespace DHE
rack::Model *modelBoosterStage =
    rack::Model::create<DHE::BoosterStage, DHE::BoosterStageWidget>(
        "DHE-Modules", "BoosterStage", "Booster Stage",
        rack::ENVELOPE_GENERATOR_TAG);
