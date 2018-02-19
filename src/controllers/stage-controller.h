#pragma once

#include <utility>

#include "util/d-flip-flop.h"
#include "util/ramp.h"
#include "util/follower.h"
#include "duration-control.h"
#include "input-port-control.h"
#include "level-control.h"
#include "output-port-control.h"
#include "shape-control.h"

namespace DHE {

struct StageController {
  StageController(
      std::function<float()> sample_time,
      LevelControl level,
      DurationControl duration,
      ShapeControl shape,
      InputPortControl defer_in,
      InputPortControl trigger_in,
      std::function<float()> stage_in,
      OutputPortControl active_out,
      OutputPortControl eoc_out,
      std::function<void(float)> stage_out
  )
      : level{std::move(level)},
        shape{std::move(shape)},
        send_active{std::move(active_out)},
        send_eoc{std::move(eoc_out)},
        send_stage{std::move(stage_out)},
        defer_gate{std::move(defer_in)},
        end_of_cycle_pulse{1e-3, sample_time},
        envelope_ramp{duration, sample_time},
        envelope_trigger{std::move(trigger_in)},
        stage_input_follower{std::move(stage_in)} {
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

    send_active(UNIPOLAR_CV.scale(is_active()));
    send_eoc(UNIPOLAR_CV.scale(end_of_cycle_pulse.is_active()));
    send_stage(defer_gate.is_high() ? stage_input_follower.value() : envelope_voltage());
  }

private:
  LevelControl level;
  ShapeControl shape;
  OutputPortControl send_active;
  OutputPortControl send_eoc;
  std::function<void(float)> send_stage;
  DFlipFlop defer_gate;
  Ramp end_of_cycle_pulse;
  Ramp envelope_ramp;
  DFlipFlop envelope_trigger;
  Follower stage_input_follower;

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
    auto range = Interval{stage_input_follower.value(), level()};
    return range.scale(shape(envelope_ramp.phase()));
  }

  bool is_active() const { return defer_gate.is_high() || envelope_ramp.is_active(); }
};

}
