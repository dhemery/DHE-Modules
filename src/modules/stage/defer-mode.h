#pragma once

namespace dhe {
namespace stage {
template <typename Controls> class DeferMode {
public:
  DeferMode(Controls &controls) : controls_{controls} {}

  void enter() { controls_.show_active(true); }
  void execute() { controls_.output(controls_.input()); }
  void exit() { controls_.show_active(false); }

private:
  Controls &controls_;
};
} // namespace stage
} // namespace dhe
