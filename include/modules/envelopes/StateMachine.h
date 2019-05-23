#pragma once
#include "Deferring.h"
#include "EndOfCyclePulseGenerator.h"
#include "Forwarding.h"
#include "Idling.h"
#include "StageState.h"

#include "modules/components/EdgeDetector.h"

#include <functional>
#include <utility>

namespace dhe {

class StateMachine {
public:
  StateMachine(std::function<bool()> deferGateIsActive,
               std::function<bool()> deferGateIsUp,
               std::function<bool()> const &stageGateIsUp,
               std::function<void()> const &startGenerating,
               std::function<void(bool)> const &setActive,
               std::function<void(bool)> const &setEoc,
               std::function<void(float)> const &forward);

  void start();
  void step(float sampleTime);

protected:
  void enter(StageState *incoming);
  void finishStage();

private:
  void onStageGateRise();
  void onStageGateFall();
  void stopDeferring();

  const std::function<bool()> deferGateIsActive;
  const std::function<bool()> stageGateIsUp;
  const std::function<void()> startGenerating;

  Deferring deferring;
  Forwarding forwarding;
  Idling idling;
  StageState *state{&forwarding};

  EdgeDetector deferGate;
  EndOfCyclePulseGenerator eocGenerator;
  EdgeDetector stageGate;
};
} // namespace dhe
