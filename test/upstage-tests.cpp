#include <catch/catch.hpp>
#include "controllers/upstage-controller.h"
#include "fake-stage-model.h"

TEST_CASE("Upstage Controller") {
  auto model = DHE::FakeStageModel{};

  DHE::UpstageController<DHE::FakeStageModel> stage{&model};
}