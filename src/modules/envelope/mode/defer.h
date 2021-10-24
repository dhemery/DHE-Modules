#pragma once

namespace dhe {
namespace envelope {
namespace mode {
template <typename Signals> class DeferMode {
public:
  DeferMode(Signals &signals) : signals_{signals} {}

  void enter() { signals_.show_active(true); }
  void execute() { signals_.output(signals_.input()); }
  void exit() { signals_.show_active(false); }

private:
  Signals &signals_;
};
} // namespace mode
} // namespace envelope
} // namespace dhe
