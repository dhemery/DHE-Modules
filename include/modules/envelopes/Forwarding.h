#pragma once
#include "StageState.h"

#include <functional>
#include <utility>

namespace dhe {

class Forwarding : public StageState {
public:
  Forwarding(std::function<void()> startGenerating, std::function<void(float)> const &forward,
             std::function<void(bool)> const &setActive) :
      StageState{
          std::move(startGenerating),         // Start generating on stage gate rise
          []() {},                            // Ignore stage gate fall
          [setActive]() { setActive(true); }, // Become active on entry
          forward                             // Forward on each step
      } {}
};
} // namespace dhe
