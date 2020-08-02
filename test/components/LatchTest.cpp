#include "components/Latch.h"

#include "doctest/doctest.h"

using dhe::Latch;

TEST_CASE("dhe::Latch") {
  SUBCASE("equality") {
    SUBCASE("true if same state and edge") {
      CHECK((Latch{false, false} == Latch{false, false}));
      CHECK((Latch{false, true} == Latch{false, true}));
      CHECK((Latch{true, false} == Latch{true, false}));
      CHECK((Latch{true, true} == Latch{true, true}));
    }

    SUBCASE("false if states differ") {
      CHECK_FALSE((Latch{false, false} == Latch{true, false}));
      CHECK_FALSE((Latch{false, true} == Latch{true, true}));
      CHECK_FALSE((Latch{true, false} == Latch{false, false}));
      CHECK_FALSE((Latch{true, true} == Latch{false, true}));
    }

    SUBCASE("false if edges differ") {
      CHECK_FALSE((Latch{false, false} == Latch{false, true}));
      CHECK_FALSE((Latch{false, true} == Latch{false, false}));
      CHECK_FALSE((Latch{true, false} == Latch{true, true}));
      CHECK_FALSE((Latch{true, true} == Latch{true, false}));
    }
  }

  SUBCASE("default") {
    Latch latch{};
    SUBCASE("is low") {
      CHECK(latch.isLow());
      CHECK_FALSE(latch.isHigh());
    }

    SUBCASE("is not edge") {
      CHECK_FALSE(latch.isEdge());
      CHECK_FALSE(latch.isRise());
      CHECK_FALSE(latch.isFall());
    }
  }

  SUBCASE("when low with no edge") {
    Latch latch{false, false};

    SUBCASE("is unchanged on low signal") {
      CHECK(latch.isLow());
      CHECK_FALSE(latch.isHigh());
      CHECK_FALSE(latch.isEdge());
      CHECK_FALSE(latch.isRise());
      CHECK_FALSE(latch.isFall());
    }

    SUBCASE("rises on high signal") {
      latch.clock(true);

      CHECK(latch.isHigh());
      CHECK_FALSE(latch.isLow());
      CHECK(latch.isEdge());
      CHECK(latch.isRise());
      CHECK_FALSE(latch.isFall());
    }
  }

  SUBCASE("when high with no edge") {
    Latch latch{true, false};

    SUBCASE("is unchanged on high signal") {
      latch.clock(true);

      CHECK_FALSE(latch.isLow());
      CHECK(latch.isHigh());
      CHECK_FALSE(latch.isEdge());
      CHECK_FALSE(latch.isRise());
      CHECK_FALSE(latch.isFall());
    }

    SUBCASE("falls on low signal") {
      latch.clock(false);

      CHECK_FALSE(latch.isHigh());
      CHECK(latch.isLow());
      CHECK(latch.isEdge());
      CHECK_FALSE(latch.isRise());
      CHECK(latch.isFall());
    }
  }

  SUBCASE("when fallen") {
    Latch latch{false, true};

    SUBCASE("loses edge on low signal") {
      latch.clock(false);

      CHECK_FALSE(latch.isHigh());
      CHECK(latch.isLow());
      CHECK_FALSE(latch.isEdge());
      CHECK_FALSE(latch.isRise());
      CHECK_FALSE(latch.isFall());
    }

    SUBCASE("rises on high signal") {
      latch.clock(true);

      CHECK(latch.isHigh());
      CHECK_FALSE(latch.isLow());
      CHECK(latch.isEdge());
      CHECK(latch.isRise());
      CHECK_FALSE(latch.isFall());
    }
  }

  SUBCASE("when risen") {
    Latch latch{true, true};

    SUBCASE("loses edge on high signal") {
      latch.clock(true);

      CHECK(latch.isHigh());
      CHECK_FALSE(latch.isLow());
      CHECK_FALSE(latch.isEdge());
      CHECK_FALSE(latch.isRise());
      CHECK_FALSE(latch.isFall());
    }

    SUBCASE("falls on low signal") {
      latch.clock(false);

      CHECK_FALSE(latch.isHigh());
      CHECK(latch.isLow());
      CHECK(latch.isEdge());
      CHECK_FALSE(latch.isRise());
      CHECK(latch.isFall());
    }
  }
}
