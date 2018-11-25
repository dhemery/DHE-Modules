#include "dhe-modules.hpp"
#include "module-widget.hpp"
#include "module.hpp"

#include "util/d-flip-flop.hpp"
#include "util/duration.hpp"
#include "util/mode.hpp"
#include "util/phase-accumulator.hpp"

namespace DHE {

struct Hostage : Module {
  enum InputIds { DEFER_IN, DURATION_CV, MAIN_IN, HOLD_GATE_IN, INPUT_COUNT };

  enum OutputIds { ACTIVE_OUT, MAIN_OUT, EOC_OUT, OUTPUT_COUNT };

  enum ParameterIds {
    DURATION_KNOB,
    DURATION_SWITCH,
    MODE_SWITCH,
    PARAMETER_COUNT
  };

  DFlipFlop sustain_gate{[this]() -> bool { return hold_in(); }};
  DFlipFlop sustain_trigger{[this]() -> bool { return hold_in(); }};

  PhaseAccumulator timer{[this] { return sample_time() / duration_in(); }};
  PhaseAccumulator eoc_pulse{[this] { return sample_time() / 1e-3f; }};

  Mode timed_sustain_mode{};
  Mode gated_sustain_mode{};
  CompoundMode sustain_mode{[this]() -> int { return mode_switch_in(); },
                            {&timed_sustain_mode, &gated_sustain_mode}};

  Mode defer_mode{};
  CompoundMode executor{[this]() -> bool { return defer_in(); },
                        {&sustain_mode, &defer_mode}};

  Hostage() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
    defer_mode.on_entry([this] { send_active(true); });
    defer_mode.on_step([this] { send_main_out(main_in()); });

    timed_sustain_mode.on_entry([this] {
      sustain_trigger.enable();
      send_active(false);
      send_main_out(main_in());
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

  void begin_sustaining() { send_active(true); }

  auto defer_in() const -> bool { return inputs[DEFER_IN].value > 0.1f; }

  auto duration_in() const -> float {
    static const auto ranges = std::vector<Range>{
        Duration::short_range, Duration::medium_range, Duration::long_range};
    auto rotation = modulated(DURATION_KNOB, DURATION_CV);
    auto selection = static_cast<int>(params[DURATION_SWITCH].value);
    auto range = ranges[selection];
    return Duration::of(rotation, range);
  }

  void end_sustaining() {
    send_active(false);
    eoc_pulse.start();
  }

  auto hold_in() const -> bool { return inputs[HOLD_GATE_IN].value > 0.1f; }

  auto main_in() const -> float { return inputs[MAIN_IN].value; }

  auto mode_switch_in() const -> int {
    return static_cast<int>(params[MODE_SWITCH].value);
  }

  auto sample_time() const -> float { return rack::engineGetSampleTime(); }

  void send_active(bool is_active) {
    outputs[ACTIVE_OUT].value = is_active ? 10.f : 0.f;
  }

  void send_eoc(bool is_pulsing) {
    outputs[EOC_OUT].value = is_pulsing ? 10.f : 0.f;
  }

  void send_main_out(float voltage) { outputs[MAIN_OUT].value = voltage; }

  void step() override { executor.step(); }
};

struct HostageWidget : public ModuleWidget {
  explicit HostageWidget(rack::Module *module)
      : ModuleWidget(module, 5, "hostage") {
    auto widget_right_edge = width();

    auto left_x = width() / 4.f + 0.333333f;
    auto center_x = widget_right_edge / 2.f;
    auto right_x = widget_right_edge - left_x;

    auto top_row_y = 25.f;
    auto row_spacing = 18.5f;

    auto row = 0;
    install_switch(Hostage::MODE_SWITCH,
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
    install_input(Hostage::MAIN_IN, {left_x, top_row_y + row * row_spacing});
    install_output(Hostage::MAIN_OUT, {right_x, top_row_y + row * row_spacing});
  }
};
} // namespace DHE
rack::Model *modelHostage =
    rack::Model::create<DHE::Hostage, DHE::HostageWidget>(
        "DHE-Modules", "Hostage", "Hostage", rack::ENVELOPE_GENERATOR_TAG);
