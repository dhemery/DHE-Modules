#include <utility>

#pragma once

namespace dhe {
class Latch {
public:
  virtual ~Latch() = default;

  virtual void step() = 0;
  virtual void set(bool state, bool edge = false) = 0;

  virtual auto isEdge() const -> bool = 0;
  virtual auto isHigh() const -> bool = 0;

  auto isLow() const -> bool { return !isHigh(); };
  auto isFall() const -> bool { return isEdge() && isLow(); };
  auto isRise() const -> bool { return isEdge() && isHigh(); };
};
} // namespace dhe