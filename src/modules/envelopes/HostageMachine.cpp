#include "modules/envelopes/HostageMachine.h"

#include "modules/components/Range.h"

#include <algorithm>

namespace dhe {

void HostageMachine::process(float sampleTime) {
  auto const newState = identifyState();
  if (state != newState) {
    enter(newState);
  }

  switch (state) {
  case Holding:
    advanceHold(sampleTime);
  case Sustaining:
  case Idle:
    sendOut(heldVoltage);
    break;
  case Deferring:
  case TrackingInput:
    sendOut(envelopeIn());
  }

  sendActive(state != Idle && state != TrackingInput);
  advanceEoc(sampleTime);
  sendEoc(isEoc);
}

HostageMachine::State HostageMachine::identifyState() {
  if (deferIsHigh()) {
    return Deferring;
  }
  if (state == Deferring) {
    return TrackingInput;
  }
  if (stageGateRise()) {
    return Holding;
  }
  return state;
}

void HostageMachine::enter(HostageMachine::State newState) {
  if (state == Deferring) {
    resetStageGate();
  }

  if (newState == Holding || newState == Sustaining) {
    resetHold();
  }
  state = newState;
}

void HostageMachine::resetStageGate() { stageGateWasHigh = false; }

bool HostageMachine::stageGateRise() {
  auto const isHigh = stageGateIsHigh();
  auto const isRise = isHigh && !stageGateWasHigh;
  stageGateWasHigh = isHigh;
  return isRise;
}

void HostageMachine::resetHold() {
  heldVoltage = envelopeIn();
  holdPhase = 0.F;
}

void HostageMachine::advanceHold(float sampleTime) {
  if (holdPhase < 1.F) {
    holdPhase = std::min(1.F, holdPhase + sampleTime / duration());
    if (holdPhase == 1.F) {
      finishHolding();
    }
  }
}

void HostageMachine::finishHolding() {
  startEoc();
  enter(Idle);
}

void HostageMachine::startEoc() {
  isEoc = true;
  eocPhase = 0.F;
}

void HostageMachine::advanceEoc(float sampleTime) {
  if (eocPhase < 1.F) {
    eocPhase = std::min(1.F, eocPhase + sampleTime / 1e-3F);
    if (eocPhase == 1.F) {
      finishEoc();
    }
  }
}

void HostageMachine::finishEoc() { isEoc = false; }
} // namespace dhe
