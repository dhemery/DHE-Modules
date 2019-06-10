#pragma once

namespace dhe {

class StageMachine {
protected:
  void process(float sampleTime);

  virtual auto envelopeIn() const -> float = 0;
  virtual auto level() const -> float = 0;
  virtual auto deferIsHigh() const -> bool = 0;
  virtual auto duration() const -> float = 0;
  virtual void sendActive(bool isActive) = 0;
  virtual void sendEoc(bool isActive) = 0;
  virtual void sendOut(float voltage) = 0;
  virtual auto taper(float input) const -> float = 0;
  virtual auto triggerIsHigh() const -> bool = 0;

private:
  enum State {
    Deferring,
    Generating,
    TrackingInput,
    TrackingLevel,
  };

  void advanceEoc(float sampleTime);
  void finishEoc();
  void finishGenerating();
  void generate(float sampleTime);
  auto identifyState() -> State;
  void resetGenerator();
  void startEoc();
  void stopDeferring();
  void enter(State newState);
  auto triggerRise() -> bool;

  float eocPhase = 1.F;
  bool isEoc = false;
  float stagePhase = 0.F;
  float startVoltage = 0.F;
  State state = TrackingInput;
  bool triggerWasHigh = false;
  void resetTrigger();
};
} // namespace dhe
