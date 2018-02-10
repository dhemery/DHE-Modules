#include <catch/catch.hpp>
#include <modules/stage.h>

TEST_CASE("Stage") {
  float sample_time{1.f / 44100.f};

  DHE::Stage stage{[] { return 0.0001;}};
}