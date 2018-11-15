#include <algorithm>

#include "dhe-modules.hpp"
#include "module-widget.hpp"
#include "module.hpp"

#include "util/d-flip-flop.hpp"
#include "util/duration.hpp"
#include "util/mode.hpp"
#include "util/ramp.hpp"

namespace DHE {

struct Hostage : Module {
  DFlipFlop sustain_gate = DFlipFlop{[this] { return hold_gate_in(); }};
  DFlipFlop sustain_trigger = DFlipFlop{[this] { return hold_gate_in(); }};
  Ramp timer = Ramp{[this] { return sample_time() / duration_in(); }};
  Ramp eoc_pulse = Ramp{[this] { return sample_time() / 1e-3f; }};

  Mode defer_mode = {};
  Mode timed_sustain_mode = {};
  Mode gated_sustain_mode = {};
  SubmodeSwitch sustain_mode = {[this] { return mode_switch_in(); },
                                &timed_sustain_mode, &gated_sustain_mode};
  SubmodeSwitch executor = {[this] { return defer_gate_in(); }, &sustain_mode,
                            &defer_mode};

  Hostage() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
    defer_mode.on_entry([this] { send_active(true); });
    defer_mode.on_step([this] { send_envelope(envelope_in()); });

    timed_sustain_mode.on_entry([this] {
      sustain_trigger.enable();
      send_active(false);
      send_envelope(envelope_in());
    });
    timed_sustain_mode.on_step([this] {
      sustain_trigger.step();
      timer.step();
    });
    timed_sustain_mode.on_exit([this] {
      sustain_trigger.disable();
      timer.stop();
    });

    sustain_trigger.on_rise([this] { timer.start(); });

    timer.on_start([this] { begin_sustaining(); });
    timer.on_completion([this] { end_sustaining(); });

    gated_sustain_mode.on_entry([this] {
      sustain_gate.step();
      sustain_gate.enable();
      if (sustain_gate.is_high())
        begin_sustaining();
      else
        end_sustaining();
    });
    gated_sustain_mode.on_step([this] { sustain_gate.step(); });
    gated_sustain_mode.on_exit([this] { sustain_gate.disable(); });

    sustain_gate.on_rise([this] { begin_sustaining(); });
    sustain_gate.on_fall([this] { end_sustaining(); });

    eoc_pulse.on_start([this] { send_eoc(true); });
    eoc_pulse.on_completion([this] { send_eoc(false); });

    executor.on_step([this] { eoc_pulse.step(); });
    executor.enter();
  }

  void end_sustaining() {
    send_active(false);
    eoc_pulse.start();
  }
  void begin_sustaining() { send_active(true); }

  void step() override { executor.step(); }

  float defer_gate_in() const { return input(DEFER_IN); };

  float mode_switch_in() const { return param(GATE_MODE_SWITCH); }

  float duration_in() const {
    const auto &range = Duration::range(param(DURATION_SWITCH));
    return Duration::scaled(duration_knob(), range);
  }

  float hold_gate_in() const { return input(HOLD_GATE_IN); }

  float envelope_in() const { return input(ENVELOPE_IN); }

  void send_active(bool is_active) {
    outputs[ACTIVE_OUT].value = is_active ? 10.f : 0.f;
  }

  void send_envelope(float voltage) { outputs[ENVELOPE_OUT].value = voltage; }

  void send_eoc(bool is_pulsing) {
    outputs[EOC_OUT].value = is_pulsing ? 10.f : 0.f;
  }

  float sample_time() const { return rack::engineGetSampleTime(); }

  std::function<float()> duration_knob = knob(DURATION_KNOB, DURATION_CV);

  enum InputIds {
    DEFER_IN,
    DURATION_CV,
    ENVELOPE_IN,
    HOLD_GATE_IN,
    INPUT_COUNT
  };

  enum OutputIds { ACTIVE_OUT, ENVELOPE_OUT, EOC_OUT, OUTPUT_COUNT };

  enum ParameterIds {
    DURATION_KNOB,
    DURATION_SWITCH,
    GATE_MODE_SWITCH,
    PARAMETER_COUNT
  };
};

struct HostageWidget : public ModuleWidget {
  HostageWidget(rack::Module *module) : ModuleWidget(module, 5, "hostage") {
    auto widget_right_edge = width();

    auto left_x = width() / 4.f + 0.333333f;
    auto center_x = widget_right_edge / 2.f;
    auto right_x = widget_right_edge - left_x;

    auto top_row_y = 25.f;
    auto row_spacing = 18.5f;

    auto row = 0;
    install_switch(Hostage::GATE_MODE_SWITCH,
                   {center_x, top_row_y + row * row_spacing});

    row++;
    install_input(Hostage::DURATION_CV,
                  {left_x, top_row_y + row * row_spacing});
    install_switch(Hostage::DURATION_SWITCH,
                   {right_x, top_row_y + row * row_spacing}, 2, 1);

    row++;
    install_knob("large", Hostage::DURATION_KNOB,
                 {center_x, top_row_y + row * row_spacing});

    top_row_y = 82.f;
    row_spacing = 15.f;

    row = 0;
    install_input(Hostage::DEFER_IN, {left_x, top_row_y + row * row_spacing});
    install_output(Hostage::ACTIVE_OUT,
                   {right_x, top_row_y + row * row_spacing});

    row++;
    install_input(Hostage::HOLD_GATE_IN,
                  {left_x, top_row_y + row * row_spacing});
    install_output(Hostage::EOC_OUT, {right_x, top_row_y + row * row_spacing});

    row++;
    install_input(Hostage::ENVELOPE_IN,
                  {left_x, top_row_y + row * row_spacing});
    install_output(Hostage::ENVELOPE_OUT,
                   {right_x, top_row_y + row * row_spacing});
  }
};
} // namespace DHE
rack::Model *modelHostage =
    rack::Model::create<DHE::Hostage, DHE::HostageWidget>(
        "DHE-Modules", "Hostage", "Hostage", rack::ENVELOPE_GENERATOR_TAG);
