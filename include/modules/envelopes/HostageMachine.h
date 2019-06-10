#pragma once

namespace dhe {

class HostageMachine {
protected:
  void process(float sampleTime);

  virtual auto envelopeIn() const -> float = 0;
  virtual auto deferIsHigh() const -> bool = 0;
  virtual auto duration() const -> float = 0;
  virtual void sendActive(bool isActive) = 0;
  virtual void sendEoc(bool isActive) = 0;
  virtual void sendOut(float voltage) = 0;
  virtual auto stageGateIsHigh() const -> bool = 0;

private:
  enum State {
    Deferring,
    Holding,
    Sustaining,
    Idle,
    TrackingInput,
  };

  void advanceEoc(float sampleTime);
  void advanceHold(float sampleTime);
  void enter(State newState);
  void finishEoc();
  void finishHolding();
  auto identifyState() -> State;
  void resetHold();
  void startEoc();
  auto stageGateRise() -> bool;

  float eocPhase = 1.F;
  bool isEoc = false;
  float holdPhase = 0.F;
  float heldVoltage = 0.F;
  State state = TrackingInput;
  bool stageGateWasHigh = false;
  void resetStageGate();
};
} // namespace dhe
