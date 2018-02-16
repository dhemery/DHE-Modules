#ifndef DHE_MODULES_CONTROLLERS_STAGE_CONTROLLER_H
#define DHE_MODULES_CONTROLLERS_STAGE_CONTROLLER_H

#include <util/d-flip-flop.h>
#include <util/ramp.h>
#include <util/follower.h>

namespace DHE {

template<typename TModule>
struct StageController {
  explicit StageController(TModule *module)
      : module{module},
        defer_gate{[this] { return defer_in(); }},
        end_of_cycle_pulse{1e-3, [this] { return sample_time(); }},
        envelope_ramp{[this] { return duration(); }, [this] { return sample_time(); }},
        envelope_trigger{[this] { return trigger_in(); }},
        stage_input_follower{[this] { return stage_in(); }} {
    defer_gate.on_rising_edge([this] { defer(); });
    defer_gate.on_falling_edge([this] { resume(); });

    envelope_trigger.on_rising_edge([this] { start_envelope(); });

    envelope_ramp.on_end_of_cycle([this] { end_of_cycle_pulse.start(); });
  }

  void step() {
    defer_gate.step();
    envelope_ramp.step();
    envelope_trigger.step();
    end_of_cycle_pulse.step();

    module->send_stage(stage_out());
    module->send_eoc(eoc_out());
    module->send_active(active_out());
  }

private:
  TModule *module;
  DFlipFlop defer_gate;
  Ramp end_of_cycle_pulse;
  Ramp envelope_ramp;
  DFlipFlop envelope_trigger;
  Follower stage_input_follower;

  float duration() const { return module->duration(); }
  float level() const { return module->level(); }
  float shape() const { return module->shape(); }
  Interval shape_range() const { return module->shape_range(); }

  float defer_in() const { return module->defer_in(); }
  float stage_in() const { return module->stage_in(); }
  float trigger_in() const { return module->trigger_in(); }

  float active_out() const { return UNIPOLAR_CV.scale(is_active()); }
  float eoc_out() const { return UNIPOLAR_CV.scale(end_of_cycle_pulse.is_active()); }
  float stage_out() const { return defer_gate.is_high() ? stage_input_follower.value() : envelope_voltage(); }

  void defer() {
    envelope_trigger.suspend_firing();
    envelope_ramp.stop();
    stage_input_follower.resume();
  }

  void resume() {
    stage_input_follower.pause();
    envelope_trigger.resume_firing();
  }

  void start_envelope() {
    stage_input_follower.pause();
    envelope_ramp.start();
  }

  float envelope_voltage() const {
    const Interval &range = shape_range();
    auto shaped{sigmoid(range.scale(envelope_ramp.phase()), shape())};

    return Interval::scale(range.normalize(shaped), stage_input_follower.value(), level());
  }

  bool is_active() const { return defer_gate.is_high() || envelope_ramp.is_active(); }

  float sample_time() const { return module->sample_time(); }
};

}

#endif
