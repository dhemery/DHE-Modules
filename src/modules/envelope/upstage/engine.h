#pragma once

namespace dhe {
namespace envelope {
namespace upstage {

template <typename Signals> class Engine {
public:
  Engine(Signals &signals) : signals_{signals} {}

  void process() {
    auto is_triggered = signals_.is_triggered() && !signals_.is_waiting();
    signals_.send_trigger(is_triggered);
    signals_.send_envelope(signals_.level());
  }

private:
  Signals &signals_;
};
} // namespace upstage
} // namespace envelope
} // namespace dhe
