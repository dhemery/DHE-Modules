#pragma once

namespace dhe {
namespace stage {
template <typename Controls> class DeferMode {
public:
  DeferMode(Controls &controls) : controls{controls} {}

  void enter() { controls.showActive(true); }
  void execute() { controls.output(controls.input()); }
  void exit() { controls.showActive(false); }

private:
  Controls &controls;
};
} // namespace stage
} // namespace dhe
