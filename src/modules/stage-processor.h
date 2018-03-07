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
      phase_0_voltage{[this] { return envelope_in(); }} {
    defer_gate.on_rising_edge([this] { on_defer_gate_rising(); });
    defer_gate.on_falling_edge([this] { on_defer_gate_falling(); });

    envelope_gate.on_rising_edge([this] { on_envelope_gate_rising(); });
    envelope_gate.on_falling_edge([this] { on_envelope_gate_falling(); });

    envelope.on_completion([this] { on_envelope_completion(); });
  }

  virtual float defer_gate_in() const = 0;
  virtual float duration_in() const = 0;
  virtual float envelope_gate_in() const = 0;
  virtual float envelope_in() const = 0;
  virtual void on_envelope_gate_falling() {}
  virtual void send_active_out(bool is_active) = 0;
  virtual void send_envelope_out(float phase_0_voltage, float phase) = 0;

  virtual void send_eoc_out(bool is_pulsing) = 0;

  virtual void on_defer_gate_falling() {
    phase_0_voltage.hold();
    envelope_gate.resume_firing();
  }

  virtual void on_envelope_gate_rising() {
    start_envelope();
  }

  bool is_active() const {
    return defer_gate.is_high() || envelope.is_active();
  }

  void on_defer_gate_rising() {
    envelope_gate.suspend_firing();
    envelope.stop();
    phase_0_voltage.track();
  }

  void on_envelope_completion() {
    eoc_pulse.start();
  }

  float sample_time() const {
    return rack::engineGetSampleTime();
  }

  void step() {
    defer_gate.step();
    envelope.step();
    envelope_gate.step();
    eoc_pulse.step();

    send_active_out(is_active());
    send_eoc_out(eoc_pulse.is_active());
    send_envelope_out(phase_0_voltage.value(), envelope.phase());
  }

  void start_envelope() {
    phase_0_voltage.hold();
    envelope.start();
  }

  DFlipFlop defer_gate;
  Ramp eoc_pulse;
  Ramp envelope;
  DFlipFlop envelope_gate;
  TrackAndHold phase_0_voltage;
};

}