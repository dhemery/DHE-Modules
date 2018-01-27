#ifndef DHE_UTIL_LATCH_H
#define DHE_UTIL_LATCH_H

#include <functional>
#include <vector>

namespace DHE {

class Latch {

public:
  bool isHigh() const { return state==HIGH; }

  bool isLow() const { return state==LOW; }

  /**
   * Suspends firing events.
   */
  void suspendFiring() {
    firingEvents = false;
  }

  /**
   * Resumes firing events.
   */
  void resumeFiring() {
    firingEvents = true;
  }

  /**
   * Registers an action to be called on each rising edge.
   * @param action called on each rising edge
   */
  void onRisingEdge(const std::function<void()> &action) {
    risingEdge.push_back(action);
  }

  /**
   * Registers an action to be called on each falling edge.
   * @param action called on each falling edge
   */
  void onFallingEdge(const std::function<void()> &action) {
    fallingEdge.push_back(action);
  }

  /**
   * Registers an action to be called when the latch is set to its current state.
   * @param action called on when the latch is set to its current state
   */
  void onNoChange(const std::function<void()> &action) {
    noChange.push_back(action);
  }

protected:
  enum State {
    UNKNOWN, LOW, HIGH
  } state = UNKNOWN;

  void setState(State newState) {
    if (state==newState) {
      fire(noChange);
    } else {
      state = newState;
      fire(state==HIGH ? risingEdge : fallingEdge);
    }
  }

private:
  bool firingEvents = true;
  std::vector<std::function<void()>> risingEdge;
  std::vector<std::function<void()>> fallingEdge;
  std::vector<std::function<void()>> noChange;

  void fire(std::vector<std::function<void()>> &actions) {
    if (!firingEvents)
      return;
    for (auto &&action : actions)
      action();
  }
};
}
#endif
