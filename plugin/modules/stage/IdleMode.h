#pragma once

namespace dhe {
namespace stage {
  template <typename Controls> class IdleMode {
  public:
    IdleMode(Controls &controls) : controls{controls} {}

    void enter() { controls.showActive(false); }
    void execute() {}
    void exit() { controls.showActive(false); }

  private:
    Controls &controls;
  };
} // namespace stage
} // namespace dhe
