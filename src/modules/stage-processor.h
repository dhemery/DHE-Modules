#pragma once

#include <engine.hpp>
#include <util/d-flip-flop.h>
#include <util/ramp.h>
#include <util/range.h>
#include <util/track-and-hold.h>

namespace DHE {

struct StageProcessor {
  StageProcessor() :
      active{},
      defer{[this] { return defer_in(); }},
      eoc{1e-3, [this] { return sample_time(); }},
      gate{[this] { return gate_in(); }},
      generator{[this] { return duration_in(); }, [this] { return sample_time(); }},
      tracker{[this] { return stage_in(); }} {
    defer.on_rising_edge([this] { begin_deferring(); });
    defer.on_falling_edge([this] { stop_deferring(); });

    gate.on_rising_edge([this] { raised_gate(); });
    gate.on_falling_edge([this] { lowered_gate(); });

    active.on_falling_edge([this] { eoc.start(); });
  }

  virtual float defer_in() const = 0;
  virtual float duration_in() const = 0;
  virtual float stage_in() const = 0;
  virtual float gate_in() const = 0;

  virtual float envelope_voltage(float held, float phase) const = 0;

  virtual bool is_end_of_cycle() const {
    return eoc.is_active();
  }

  virtual bool is_active() const {
    return defer.is_high() || generator.is_active();
  }

  virtual void raised_gate() {
    start_envelope();
  }

  virtual void lowered_gate() {
  }

  virtual void send_active_out(float f) = 0;
  virtual void send_eoc_out(float f) = 0;
  virtual void send_stage_out(float f) = 0;

  void step() {
    defer.step();
    generator.step();
    gate.step();
    if (is_active()) {
      active.set();
    } else {
      active.reset();
    }
    eoc.step();

    send_active_out(active_out());
    send_eoc_out(eoc_out());
    send_stage_out(stage_out());
  }

  float active_out() const {
    return UNIPOLAR_SIGNAL_RANGE.scale(active.is_high());
  }

  virtual void begin_deferring() {
    gate.suspend_firing();
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
    return defer.is_high() ? tracker.value() : envelope_voltage(tracker.value(), generator.phase());
  }

  void start_envelope() {
    tracker.hold();
    generator.start();
  }

  void stop_deferring() {
    tracker.hold();
    gate.resume_firing();
  }

  DLatch active;
  DFlipFlop defer;
  Ramp eoc;
  DFlipFlop gate;
  Ramp generator;
  TrackAndHold tracker;
};

}