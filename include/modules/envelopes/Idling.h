#pragma once
#include "StageState.h"

#include <functional>
#include <utility>

namespace dhe {

class Idling : public StageState {
public:
  explicit Idling(std::function<void()> startGenerating,
                  std::function<void(bool)> const &setActive) :
      StageState{
          std::move(startGenerating),          // Start generating on stage gate rise
          []() {},                             // Ignore stage gate fall
          [setActive]() { setActive(false); }, // Become inactive on entry
          [](float) {}                         // Do nothing on each step
      } {}
};
} // namespace dhe
