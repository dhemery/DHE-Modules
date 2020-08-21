#include "components/phase-timer.h"

#include <doctest.h>

using dhe::PhaseTimer;

TEST_CASE("dhe::PhaseTimer") {
  SUBCASE("default") {
    PhaseTimer timer{};
    SUBCASE("phase is 0") { CHECK(timer.phase() == 0.F); }

    SUBCASE("is in progress") { CHECK(timer.in_progress()); }
  }

  SUBCASE("advance") {
    PhaseTimer timer{};
    SUBCASE("adds delta to phase") {
      timer.advance(0.38F);

      CHECK_EQ(timer.phase(), 0.38F);

      timer.advance(0.19F);

      CHECK_EQ(timer.phase(), 0.57F);
    }
  }

  SUBCASE("in progress") {
    SUBCASE("true if phase is less than 1") {
      PhaseTimer timer{0.9999F};

      CHECK(timer.in_progress());
    }
    SUBCASE("false if phase is at least 1") {
      PhaseTimer timer{1.F};

      CHECK_FALSE(timer.in_progress());
    }
  }

  SUBCASE("maximum phase is 1") {
    PhaseTimer timer{1234.56789F};
    CHECK_EQ(timer.phase(), 1.F);

    timer.advance(1234.56789F);
    CHECK_EQ(timer.phase(), 1.F);
  }

  SUBCASE("reset") {
    SUBCASE("sets phase to 0") {
      PhaseTimer timer{0.74F};

      timer.reset();

      CHECK_EQ(timer.phase(), 0.F);
    }

    SUBCASE("leaves timer in progress") {
      PhaseTimer timer{1.F};

      timer.reset();

      CHECK(timer.in_progress());
    }
  }
}
