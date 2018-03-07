#pragma once

#include <engine.hpp>
#include <util/d-flip-flop.h>
#include <util/ramp.h>
#include <util/range.h>
#include <util/track-and-hold.h>

namespace DHE {

struct StageProcessor {
  StageProcessor() :
      defer_gate{[this] { return defer_gate_in(); }},
      eoc_pulse{1e-3, [this] { return sample_time(); }},
      envelope{[this] { return duration_in(); }, [this] { return sample_time(); }},
      envelope_gate{[this] { return envelope_gate_in(); }},
      envelope_start{[this] { return envelope_in(); }} {
    defer_gate.on_rising_edge([this] { on_defer_rising(); });
    defer_gate.on_falling_edge([this] { on_defer_falling(); });

    envelope_gate.on_rising_edge([this] { on_gate_rising(); });
    envelope_gate.on_falling_edge([this] { on_gate_falling(); });

    envelope.on_completion([this] { on_envelope_completion(); });
  }

  virtual float defer_gate_in() const = 0;
  virtual float duration_in() const = 0;
  virtual float envelope_in() const = 0;
  virtual float envelope_gate_in() const = 0;
  virtual float envelope_voltage(float start_voltage, float phase) const = 0;
  virtual void send_active_out(bool is_active) = 0;
  virtual void send_eoc_out(bool is_pulsing) = 0;
  virtual void send_envelope_out(float envelope_out) = 0;

  virtual void on_defer_rising() {
    envelope_gate.suspend_firing();
    envelope.stop();
    envelope_start.track();
  }

  virtual void on_defer_falling() {
    envelope_start.hold();
    envelope_gate.resume_firing();
  }

  virtual void on_envelope_completion() {
    eoc_pulse.start();
  }

  virtual void on_gate_falling() {}

  virtual void on_gate_rising() {
    start_envelope();
  }

  void step() {
    defer_gate.step();
    envelope.step();
    envelope_gate.step();
    eoc_pulse.step();

    send_active_out(is_active());
    send_eoc_out(eoc_pulse.is_active());
    send_envelope_out(stage_out());
  }

  bool is_active() const {
    return defer_gate.is_high() || envelope.is_active();
  }

  float sample_time() const {
    return rack::engineGetSampleTime();
  }

  float stage_out() const {
    return defer_gate.is_high() ? envelope_start.value() : envelope_voltage(envelope_start.value(), envelope.phase());
  }

  void start_envelope() {
    envelope_start.hold();
    envelope.start();
  }

  DFlipFlop defer_gate;
  Ramp eoc_pulse;
  Ramp envelope;
  DFlipFlop envelope_gate;
  TrackAndHold envelope_start;
};

}