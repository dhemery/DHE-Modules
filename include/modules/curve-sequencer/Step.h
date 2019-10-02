#pragma once

namespace dhe {
namespace curve_sequencer {
  class Step {
  public:
    enum Mode { Rise, Fall, Edge, High, Low, Skip, Duration };

    virtual auto isAvailable() const -> bool = 0;
    virtual void process(float sampleTime) = 0;
    virtual ~Step() = default;
  };
} // namespace curve_sequencer
} // namespace dhe
