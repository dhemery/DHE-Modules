#pragma once

namespace dhe {
namespace stage {
template <typename Controls> class LevelMode {
public:
  LevelMode(Controls &controls) : controls{controls} {}

  void enter() { controls.showActive(false); }
  void execute() { controls.output(controls.level()); }
  void exit() { controls.showActive(false); }

private:
  Controls &controls;
};
} // namespace stage
} // namespace dhe
