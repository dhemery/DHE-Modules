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
      eoc{1e-3, [this] { return sample_time(); }},
      generator{[this] { return duration_in(); }, [this] { return sample_time(); }},
      trigger{[this] { return trigger_in(); }},
      tracker{[this] { return stage_in(); }} {
    defer_gate.on_rising_edge([this] { begin_deferring(); });
    defer_gate.on_falling_edge([this] { stop_deferring(); });

    trigger.on_rising_edge([this] { triggered(); });

    generator.on_end_of_cycle([this] { eoc.start(); });
  }


  virtual float defer_in() const = 0;
  virtual float duration_in() const = 0;
  virtual float stage_in() const = 0;
  virtual float trigger_in() const = 0;

  virtual float envelope_voltage(float held, float phase) const = 0;

  virtual bool is_end_of_cycle() const {
    return eoc.is_active();
  }

  virtual bool is_active() const {
    return defer_gate.is_high() || generator.is_active();
  }

  virtual void triggered() {
    start_envelope();
  }

  virtual void send_active_out(float f) = 0;
  virtual void send_eoc_out(float f) = 0;
  virtual void send_stage_out(float f) = 0;

  void step() {
    defer_gate.step();
    generator.step();
    trigger.step();
    eoc.step();

    send_active_out(active_out());
    send_eoc_out(eoc_out());
    send_stage_out(stage_out());
  }

  float active_out() const {
    return UNIPOLAR_SIGNAL_RANGE.scale(is_active());
  }

  virtual void begin_deferring() {
    trigger.suspend_firing();
    generator.stop();
    tracker.track();
  }

  float eoc_out() const {
    return UNIPOLAR_SIGNAL_RANGE.scale(is_end_of_cycle());
  }

  float sample_time() const {
    return rack::engineGetSampleTime();
  }

  float stage_out() const {
    return defer_gate.is_high() ? tracker.value() : envelope_voltage(tracker.value(), generator.phase());
  }

  void start_envelope() {
    tracker.hold();
    generator.start();
  }

  void stop_deferring() {
    tracker.hold();
    trigger.resume_firing();
  }

  DFlipFlop defer_gate;
  Ramp eoc;
  Ramp generator;
  DFlipFlop trigger;
  TrackAndHold tracker;
};

}