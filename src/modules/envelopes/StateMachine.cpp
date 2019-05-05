#include "modules/envelopes/StateMachine.h"

namespace DHE {

StateMachine::StateMachine(std::function<bool()> defer_gate_is_active,
                           std::function<bool()> defer_gate_is_up,
                           std::function<bool()> const &stage_gate_is_up,
                           std::function<void()> const &start_generating,
                           std::function<void(bool)> const &set_active,
                           std::function<void(bool)> const &set_eoc,
                           std::function<void(float)> const &forward)
    : defer_gate_is_active{std::move(defer_gate_is_active)},
      stage_gate_is_up{stage_gate_is_up}, start_generating{start_generating},
      deferring{forward, set_active}, forwarding{start_generating, forward,
                                                 set_active},
      idling{start_generating, set_active}, defer_gate{
                                                std::move(defer_gate_is_up),
                                                [this]() { enter(&deferring); },
                                                [this]() { stop_deferring(); }},
      eoc_generator{[set_eoc]() { set_eoc(true); },
                    [set_eoc]() { set_eoc(false); }},
      stage_gate{stage_gate_is_up, [this]() { on_stage_gate_rise(); },
                 [this]() { on_stage_gate_fall(); }} {}

void StateMachine::enter(StageState *incoming) {
  state = incoming;
  state->enter();
}

void StateMachine::finish_stage() {
  eoc_generator.start();
  enter(&idling);
}

void StateMachine::on_stage_gate_fall() { state->on_stage_gate_fall(); }

void StateMachine::on_stage_gate_rise() {
  // If DEFER is active, ignore GATE rises.
  // We will check GATE when DEFER falls.
  if (defer_gate_is_active())
    return;
  state->on_stage_gate_rise();
}

void StateMachine::start() { state->enter(); }

void StateMachine::step(float sampleTime) {
  defer_gate.step();
  stage_gate.step();
  state->step(sampleTime);
  eoc_generator.step(sampleTime);
}

void StateMachine::stop_deferring() {
  if (stage_gate_is_up()) {
    start_generating();
  } else {
    finish_stage();
  }
}
} // namespace DHE
