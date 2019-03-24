#pragma once

#include <stage/components/defer-gate.h>
#include <stage/components/eoc-generator.h>
#include <stage/components/stage-gate.h>
#include <stage/components/stage-generator.h>
#include <stage/modes/stage-mode.h>
#include <stage/modes/deferring-mode.h>
#include <stage/modes/following-mode.h>
#include <stage/modes/generating-mode.h>

namespace DHE {

class StageModule : public rack::Module {
public:
  StageModule(int parameterCount, int inputCount, int outputCount)
  : rack::Module{parameterCount, inputCount, outputCount} {
    mode->enter();
  }

  virtual auto defer_gate_in() const -> bool = 0;
  virtual auto stage_gate_in() const -> bool = 0;

  virtual auto duration() const -> float = 0;
  virtual auto envelope_in() const -> float = 0;
  virtual auto level() const -> float = 0;

  virtual void set_active(bool b) = 0;
  virtual void set_eoc(bool b) = 0;
  virtual void send_phase(float phase) = 0;
  virtual void send_out(float voltage) = 0;

  void step() override {
    defer_gate.step();
    stage_gate.step();
    mode->step();
    eoc_generator.step();
  }

  void on_defer_gate_rise() { mode->on_defer_gate_rise(); }
  virtual void on_defer_gate_fall() { mode->on_defer_gate_fall(); }

  auto sampleTime() const -> float { return rack::engineGetSampleTime(); }

  void on_stage_gate_rise() { mode->on_stage_gate_rise(); }
  void on_stage_gate_fall() { mode->on_stage_gate_fall(); }

  void begin_generating() {
    set_active(true);
    hold_input();
    stage_generator.start();
    enter(&generating_mode);
  }

  void begin_deferring() {
    set_active(true);
    enter(&deferring_mode);
  }

  void begin_following() {
    set_active(false);
    enter(&following_mode);
  }

  void on_eoc_start() { set_eoc(true); }
  void on_eoc_end() { set_eoc(false); }

  void do_defer() { send_input(); }
  void do_follow() { send_level(); }
  void do_generate() { stage_generator.step(); }

  virtual void on_generate_start() {}
  virtual void on_generate_end() {
    eoc_generator.start();
    begin_following();
  }

  auto held_voltage() const -> float { return held; }

private:
  void enter(StageMode<StageModule> *incoming) {
    mode->exit();
    mode = incoming;
    mode->enter();
  }

  void hold_input() { held = envelope_in(); }
  void send_input() { send_out(envelope_in()); }
  void send_level() { send_out(level()); }


  DeferringMode<StageModule> deferring_mode{this};
  GeneratingMode<StageModule> generating_mode{this};
  FollowingMode<StageModule> following_mode{this};

  StageMode <StageModule> *mode{&following_mode};

  DeferGate<StageModule> defer_gate{this};
  StageGate<StageModule> stage_gate{this};
  EocGenerator<StageModule> eoc_generator{this};
  StageGenerator<StageModule> stage_generator{this};

  float held = 0.f;
};
}