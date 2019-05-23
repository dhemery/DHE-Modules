#pragma once
#include <functional>
#include <utility>

namespace dhe {

class StageState {
public:
  explicit StageState(std::function<void()> onStageGateRise,
                      std::function<void()> onStageGateFall,
                      std::function<void()> onEntry,
                      std::function<void(float)> onStep)
      : onStageGateRise{std::move(onStageGateRise)}, onStageGateFall{std::move(
                                                         onStageGateFall)},
        enter{std::move(onEntry)}, step{std::move(onStep)} {}
  const std::function<void()> onStageGateRise;
  const std::function<void()> onStageGateFall;
  const std::function<void()> enter;
  const std::function<void(float)> step;
};
} // namespace dhe
