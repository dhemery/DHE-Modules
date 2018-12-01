#pragma once

#include <engine.hpp>

namespace DHE {
class PhaseGenerator {
public:
  void start() {
    accumulated = 0.f;
    on_start();
  }

  void step() {
    accumulated += rack::engineGetSampleTime() / duration();
    if (accumulated >= 1.0f) {
      accumulated = 1.f;
    };
    if (accumulated >= 1.0f) {
      on_completion();
    };
  }

  auto phase() const -> float { return this->accumulated; }

protected:
  virtual void on_start() const {};
  virtual auto duration() const -> float = 0;
  virtual void on_completion() const {};

private:
  float accumulated = 0.f;
};
} // namespace DHE
