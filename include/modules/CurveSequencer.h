#pragma once

namespace dhe {
class CurveSequencer {
public:
  virtual auto gate() -> int = 0;
  virtual auto isRunning() -> bool = 0;
  virtual void send(float voltage) = 0;
  virtual auto startStep() -> int = 0;
};

} // namespace dhe