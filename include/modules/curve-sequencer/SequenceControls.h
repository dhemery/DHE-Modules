#pragma once

namespace dhe {

namespace curve_sequencer {
  class SequenceControls {
  public:
    virtual auto gate() const -> bool = 0;
    virtual auto isRunning() const -> bool = 0;
    virtual auto selectionLength() const -> int = 0;
    virtual auto selectionStart() const -> int = 0;
  };
} // namespace curve_sequencer
} // namespace dhe
