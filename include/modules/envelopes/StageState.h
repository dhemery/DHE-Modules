#pragma once
#include <functional>
#include <utility>

namespace dhe {

class StageState {
public:
  explicit StageState(std::function<void()> onStageGateRise, std::function<void()> onStageGateFall,
                      std::function<void()> onEntry, std::function<void(float)> onStep) :
      stageGateRiseAction{std::move(onStageGateRise)},
      stageGateFallAction{std::move(onStageGateFall)},
      enterAction{std::move(onEntry)},
      stepAction{std::move(onStep)} {}

  void onStageGateRise() const { stageGateRiseAction(); }
  void onStageGateFall() const { stageGateFallAction(); }
  void enter() const { enterAction(); }
  void step(float sampleTime) const { stepAction(sampleTime); }

private:
  const std::function<void()> stageGateRiseAction;
  const std::function<void()> stageGateFallAction;
  const std::function<void()> enterAction;
  const std::function<void(float)> stepAction;
};
} // namespace dhe
