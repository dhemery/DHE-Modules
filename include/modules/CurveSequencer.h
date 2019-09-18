#pragma once

namespace dhe {
class CurveSequencer {
public:
  virtual auto isRunning() -> bool = 0;
  virtual void send(float voltage) = 0;
};

} // namespace dhe