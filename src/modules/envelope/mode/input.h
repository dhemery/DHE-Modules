#pragma once

namespace dhe {
namespace envelope {
namespace mode {
template <typename Controls> class InputMode {
public:
  InputMode(Controls &controls) : controls_{controls} {}

  void enter() { controls_.show_active(false); }
  void execute() { controls_.output(controls_.input()); }
  void exit() { controls_.show_active(false); }

private:
  Controls &controls_;
};
} // namespace mode
} // namespace envelope
} // namespace dhe
