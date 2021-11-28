#pragma once

namespace dhe {
namespace envelope {
template <typename Signals> class LevelMode {
public:
  LevelMode(Signals &signals) : signals_{signals} {}

  void enter() { signals_.show_active(false); }
  void execute() { signals_.output(signals_.level()); }
  void exit() { signals_.show_active(false); }

private:
  Signals &signals_;
};

} // namespace envelope
} // namespace dhe
