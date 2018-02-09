#include "catch.hpp"
#include "util/follower.h"

TEST_CASE("Follower") {
  constexpr float initial_value{93.f};
  float source{initial_value};
  auto supplier = [&source] { return source; };
  DHE::Follower follower{supplier};

  SECTION("newly constructed") {

    SECTION("yields the initially supplied value") {
      REQUIRE(follower.value() == initial_value);
    }

    SECTION("does not follow the supplier") {
      source += 1;

      REQUIRE(follower.value() != supplier());
    }
  }

  SECTION("when following") {
    follower.resume();

    SECTION("yields the supplier's value") {
      source += 1;
      REQUIRE(follower.value() == supplier());
    }
  }

  SECTION("when paused") {
    constexpr float value_at_pause{543.f};
    source = value_at_pause;
    follower.pause();

    source += 1;

    SECTION("yields the value supplied at time of pause") {
      REQUIRE(follower.value() == value_at_pause);
    }

    SECTION("does not follow the supplier") {
      REQUIRE(follower.value() != supplier());
    }
  }
}