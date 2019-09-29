#pragma once

#include "Latch.h"

#include <functional>
#include <utility>

namespace dhe {
class FunctionLatch : public Latch {
public:
  FunctionLatch(std::function<bool()> supplier) : getState{std::move(supplier)} {}
  ~FunctionLatch() override = default;

  auto isEdge() const -> bool override { return edge; }

  auto isHigh() const -> bool override { return state; }

  void set(bool state, bool edge) override {
    this->state = state;
    this->edge = edge;
  }
  void step() override {
    auto const newState = getState();
    edge = state != newState;
    state = newState;
  }

private:
  std::function<bool()> const getState;
  bool state{false};
  bool edge{false};
};
} // namespace dhe