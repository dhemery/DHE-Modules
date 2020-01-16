#pragma once

namespace dhe {
namespace stage {
  template <typename Controls> class InputMode {
  public:
    InputMode(Controls &controls) : controls{controls} {}

    void enter() { controls.showActive(false); }
    void execute() { controls.output(controls.input()); }
    void exit() { controls.showActive(false); }

  private:
    Controls &controls;
  };
} // namespace stage
} // namespace dhe
