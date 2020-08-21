#pragma once

#include "components/latch.h"

#include <string>
#include <utility>

namespace test {
namespace stage {
using dhe::Latch;
using dhe::stage::Event;

class FakeMode {
  bool active{};
  bool entered{};
  bool exited{};

protected:
  bool executed{};
  Event event{};

public:
  void enter() {
    entered = true;
    active = true;
  }
  void exit() {
    exited = true;
    active = false;
  }

  void returns(Event e) { event = e; }
  auto isActive() const -> bool { return active; }
  auto wasEntered() const -> bool { return entered; }
  auto wasExecuted() const -> bool { return executed; }
  auto wasExited() const -> bool { return exited; }

  virtual void reset() {
    entered = false;
    executed = false;
    exited = false;
  }
};

class FakeSimpleMode : public FakeMode {
public:
  auto execute() -> Event {
    executed = true;
    return event;
  }
};

class FakeLatchedMode : public FakeMode {
  Latch executedLatch;

public:
  auto execute(Latch const &latch) -> Event {
    executed = true;
    executedLatch = Latch{latch.is_high(), latch.is_edge()};
    return event;
  }

  auto latch() const -> Latch { return executedLatch; }
};

class FakeTimedMode : public FakeMode {
  Latch executedLatch{};
  float executedSampleTime{};

public:
  auto execute(Latch const &latch, float sampleTime) -> Event {
    executed = true;
    executedLatch = Latch{latch.is_high(), latch.is_edge()};
    executedSampleTime = sampleTime;
    return event;
  }

  auto latch() const -> Latch { return executedLatch; }

  auto sampleTime() const -> float { return executedSampleTime; }
};
} // namespace stage
} // namespace test
