#include "components/Latch.h"

#include "doctest.h"

using dhe::Latch;

TEST_CASE("latches are equal if same state and edge") {
  CHECK((Latch{false, false} == Latch{false, false}));
  CHECK((Latch{false, true} == Latch{false, true}));
  CHECK((Latch{true, false} == Latch{true, false}));
  CHECK((Latch{true, true} == Latch{true, true}));
}

TEST_CASE("latches are unequal if states differ") {
  CHECK(!(Latch{false, false} == Latch{true, false}));
  CHECK(!(Latch{false, true} == Latch{true, true}));
  CHECK(!(Latch{true, false} == Latch{false, false}));
  CHECK(!(Latch{true, true} == Latch{false, true}));
}

TEST_CASE("latches are unequal if edges differ") {
  CHECK(!(Latch{false, false} == Latch{false, true}));
  CHECK(!(Latch{false, true} == Latch{false, false}));
  CHECK(!(Latch{true, false} == Latch{true, true}));
  CHECK(!(Latch{true, true} == Latch{true, false}));
}

TEST_CASE("latch") {
  Latch latch{};

  SUBCASE("when new") {
    SUBCASE("is low") {
      CHECK(latch.isLow());
      CHECK(!latch.isHigh());
    }

    SUBCASE("is not edge") {
      CHECK(!latch.isEdge());
      CHECK(!latch.isRise());
      CHECK(!latch.isFall());
    }
  }

  SUBCASE("when low with no edge") {
    SUBCASE("is unchanged on low signal") {
      latch.clock(false);
      latch.clock(false); // to remove edge

      CHECK(latch.isLow());
      CHECK(!latch.isHigh());
      CHECK(!latch.isEdge());
      CHECK(!latch.isRise());
      CHECK(!latch.isFall());
    }

    SUBCASE("rises on high signal") {
      latch.clock(true);
      CHECK(latch.isHigh());
      CHECK(!latch.isLow());
      CHECK(latch.isEdge());
      CHECK(latch.isRise());
      CHECK(!latch.isFall());
    }
  }

  SUBCASE("when high with no edge") {
    latch.clock(true);
    latch.clock(true); // to remove edge

    SUBCASE("is unchanged on high signal") {
      latch.clock(true);

      CHECK(!latch.isLow());
      CHECK(latch.isHigh());
      CHECK(!latch.isEdge());
      CHECK(!latch.isRise());
      CHECK(!latch.isFall());
    }

    SUBCASE("falls on low signal") {
      latch.clock(false);
      CHECK(!latch.isHigh());
      CHECK(latch.isLow());
      CHECK(latch.isEdge());
      CHECK(!latch.isRise());
      CHECK(latch.isFall());
    }
  }

  SUBCASE("when fallen") {
    latch.clock(true);
    latch.clock(false);

    SUBCASE("loses edge on low signal") {
      latch.clock(false);

      CHECK(!latch.isHigh());
      CHECK(latch.isLow());
      CHECK(!latch.isEdge());
      CHECK(!latch.isRise());
      CHECK(!latch.isFall());
    }

    SUBCASE("rises on high signal") {
      latch.clock(true);
      CHECK(latch.isHigh());
      CHECK(!latch.isLow());
      CHECK(latch.isEdge());
      CHECK(latch.isRise());
      CHECK(!latch.isFall());
    }
  }

  SUBCASE("when risen") {
    latch.clock(false);
    latch.clock(true);

    SUBCASE("loses edge on high signal") {
      latch.clock(true);

      CHECK(latch.isHigh());
      CHECK(!latch.isLow());
      CHECK(!latch.isEdge());
      CHECK(!latch.isRise());
      CHECK(!latch.isFall());
    }

    SUBCASE("falls on low signal") {
      latch.clock(false);
      CHECK(!latch.isHigh());
      CHECK(latch.isLow());
      CHECK(latch.isEdge());
      CHECK(!latch.isRise());
      CHECK(latch.isFall());
    }
  }
}
