#ifndef DHE_UTIL_FOLLOWER_H
#define DHE_UTIL_FOLLOWER_H

#include <functional>
#include <utility>

namespace DHE {

/**
 * A follower operates in one of two modes: following and paused.
 * While following, a follower yields the supplied signal.
 * While paused, it yields the signal value it stored when it paused.
 *
 * A newly constructed follower is paused.
 */
struct Follower {

  /**
   * Creates a follower that follows the supplied signal.
   * @param signal supplies the signal to follow
   */
  explicit Follower(std::function<float()> signal) :
      signal{std::move(signal)},
      stored{this->signal()},
      following{false} {}

  /**
   * Stores the value of the supplied signal and pauses. Subsequent calls to
   * value() yield the stored value.
   */
  void pause() {
    stored = signal();
    following = false;
  }

  /**
   * Resumes following the supplied signal. Subsequent calls to value()
   * yield the supplied signal value.
   */
  void resume() {
    following = true;
  }

  /**
   * Returns the supplied signal value if the follower is in follower mode.
  // Otherwise returns the value stored by the previous pause().
   * @return the supplied signal value if the follower is following,
   * otherwise the stored value.
   */
  float value() const {
    return following ? signal() : stored;
  }

private:
  const std::function<float()> signal;
  float stored;
  bool following;
};
}
#endif
