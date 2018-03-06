#pragma once

#include <engine.hpp>
#include <util/d-flip-flop.h>
#include <util/ramp.h>
#include <util/range.h>
#include <util/track-and-hold.h>

namespace DHE {

struct StageProcessor {
  StageProcessor() :
      defer_gate{[this] { return defer_in(); }},
      end_of_cycle_pulse{1e-3, [this] { return sample_time(); }},
      envelope_ramp{[this] { return duration_in(); }, [this] { return sample_time(); }},
      envelope_trigger{[this] { return trigger_in(); }},
      stage_in_th{[this] { return stage_in(); }} {
    defer_gate.on_rising_edge([this] { begin_deferring(); });
    defer_gate.on_falling_edge([this] { stop_deferring(); });

    envelope_trigger.on_rising_edge([this] { start_envelope(); });

    envelope_ramp.on_end_of_cycle([this] { end_of_cycle_pulse.start(); });
  }

  virtual float defer_in() const = 0;
  virtual float duration_in() const = 0;
  virtual float level_in() const = 0;
  virtual float stage_in() const = 0;


  virtual bool is_end_of_cycle() const {
    return end_of_cycle_pulse.is_active();
  }

  virtual bool is_active() const {
    return defer_gate.is_high() || envelope_ramp.is_active();
  }

  virtual float trigger_in() const = 0;
  virtual float taper(float phase) const = 0;

  virtual void send_active_out(float f) = 0;
  virtual void send_eoc_out(float f) = 0;
  virtual void send_stage_out(float f) = 0;

  void process() {
    defer_gate.step();
    envelope_ramp.step();
    envelope_trigger.step();
    end_of_cycle_pulse.step();

    send_active_out(active_out());
    send_eoc_out(eoc_out());
    send_stage_out(stage_out());
  }

private:
  float active_out() const {
    return UNIPOLAR_SIGNAL_RANGE.scale(is_active());
  }

  void begin_deferring() {
    envelope_trigger.suspend_firing();
    envelope_ramp.stop();
    stage_in_th.track();
  }

  float envelope_voltage() const {
    auto range = Range{stage_in_th.value(), level_in()};
    return range.scale(taper(envelope_ramp.phase()));
  }

  float eoc_out() const {
    return UNIPOLAR_SIGNAL_RANGE.scale(is_end_of_cycle());
  }

  float sample_time() const {
    return rack::engineGetSampleTime();
  }

  float stage_out() const {
    return defer_gate.is_high() ? stage_in_th.value() : envelope_voltage();
  }

  void stop_deferring() {
    stage_in_th.hold();
    envelope_trigger.resume_firing();
  }

  void start_envelope() {
    stage_in_th.hold();
    envelope_ramp.start();
  }

  DFlipFlop defer_gate;
  Ramp end_of_cycle_pulse;
  Ramp envelope_ramp;
  DFlipFlop envelope_trigger;
  TrackAndHold stage_in_th;
};

}