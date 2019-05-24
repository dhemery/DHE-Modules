#include "modules/envelopes/StateMachine.h"

namespace dhe {

StateMachine::StateMachine(std::function<bool()> deferGateIsActive, std::function<bool()> deferGateIsUp,
                           std::function<bool()> const &stageGateIsUp, std::function<void()> const &startGenerating,
                           std::function<void(bool)> const &setActive, std::function<void(bool)> const &setEoc,
                           std::function<void(float)> const &forward) :
    deferGateIsActive{std::move(deferGateIsActive)},
    stageGateIsUp{stageGateIsUp},
    startGenerating{startGenerating},
    deferring{forward, setActive},
    forwarding{startGenerating, forward, setActive},
    idling{startGenerating, setActive},
    deferGate{std::move(deferGateIsUp), [this]() { enter(&deferring); }, [this]() { stopDeferring(); }},
    eocGenerator{[setEoc]() { setEoc(true); }, [setEoc]() { setEoc(false); }},
    stageGate{stageGateIsUp, [this]() { onStageGateRise(); }, [this]() { onStageGateFall(); }} {}

void StateMachine::enter(StageState *incoming) {
  state = incoming;
  state->enter();
}

void StateMachine::finishStage() {
  eocGenerator.start();
  enter(&idling);
}

void StateMachine::onStageGateFall() { state->onStageGateFall(); }

void StateMachine::onStageGateRise() {
  // If DEFER is active, ignore GATE rises.
  // We will check GATE when DEFER falls.
  if (deferGateIsActive()) {
    return;
  }
  state->onStageGateRise();
}

void StateMachine::start() { state->enter(); }

void StateMachine::step(float sampleTime) {
  deferGate.step();
  stageGate.step();
  state->step(sampleTime);
  eocGenerator.step(sampleTime);
}

void StateMachine::stopDeferring() {
  if (stageGateIsUp()) {
    startGenerating();
  } else {
    finishStage();
  }
}
} // namespace dhe
