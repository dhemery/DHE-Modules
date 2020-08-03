#pragma once

#include "components/Latch.h"

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

  class SimpleFakeMode : public FakeMode {
  public:
    auto execute() -> Event {
      executed = true;
      return event;
    }
  };

  class LatchedFakeMode : public FakeMode {
    Latch executedlatch;

  public:
    auto execute(Latch const &latch) -> Event {
      executed = true;
      executedlatch = latch;
      return event;
    }

    auto latch() const -> Latch { return executedlatch; }
  };

  class TimedFakeMode : public FakeMode {
    Latch executedLatch{};
    float executedSampleTime{};

  public:
    auto execute(Latch const &latch, float sampleTime) -> Event {
      executed = true;
      executedLatch = latch;
      executedSampleTime = sampleTime;
      return event;
    }

    auto latch() const -> Latch { return executedLatch; }

    auto sampleTime() const -> float { return executedSampleTime; }
  };
} // namespace stage
} // namespace test
