#include <catch/catch.hpp>
#include <modules/upstage-module.h>

static const auto LEVEL_SWITCH_UNI = 1;
static const auto LEVEL_SWITCH_BI = 0;
static const auto MAX_CV = 5.f;
static const auto MIN_CV = -5.f;

static Catch::Matchers::Floating::WithinUlpsMatcher near(float expected) {
  return Catch::Matchers::WithinULP(expected, 1);
}

TEST_CASE("Upstage") {
  auto upstage = DHE::UpstageModule{};
  auto &level_switch = upstage.params[upstage.LEVEL_SWITCH].value;
  auto &level_knob = upstage.params[upstage.LEVEL_KNOB].value;
  auto &out = upstage.outputs[upstage.ENVELOPE_OUT].value;

  SECTION("level") {
    SECTION("unipolar range") {
      auto const selected_range = DHE::UNIPOLAR_SIGNAL_RANGE;
      level_switch = LEVEL_SWITCH_UNI;

      SECTION("max rotation yields max unipolar signal") {
        level_knob = 1.f;
        upstage.step();
        REQUIRE_THAT(out, near(selected_range.upper_bound));
      }

      SECTION("min rotation yields min unipolar signal") {
        level_knob = 0.f;
        upstage.step();
        REQUIRE_THAT(out, near(selected_range.lower_bound));
      }

      SECTION("center rotation yields central unipolar signal ") {
        level_knob = 0.5f;
        upstage.step();
        REQUIRE_THAT(out, near(selected_range.scale(level_knob)));
      }
    }

    SECTION("bipolar range") {
      auto const selected_range = DHE::cv_range;
      level_switch = LEVEL_SWITCH_BI;

      SECTION("max rotation yields max bipolar signal") {
        level_knob = 1.f;
        upstage.step();
        REQUIRE_THAT(out, near(selected_range.upper_bound));
      }

      SECTION("min rotation yields min bipolar signal") {
        level_knob = 0.f;
        upstage.step();
        REQUIRE_THAT(out, near(selected_range.lower_bound));
      }

      SECTION("center rotation yields central bipolar signal") {
        level_knob = 0.5f;
        upstage.step();
        REQUIRE_THAT(out, near(selected_range.scale(0.5f)));
      }
    }

    SECTION("modulation") {
      auto &level_cv = upstage.inputs[upstage.LEVEL_CV].value;
      level_switch = LEVEL_SWITCH_UNI;
      auto const selected_range = DHE::UNIPOLAR_SIGNAL_RANGE;

      SECTION("max cv adds half rotation") {
        level_knob = 0.35f;
        level_cv = MAX_CV;
        upstage.step();
        REQUIRE_THAT(out, near(selected_range.scale(level_knob + 0.5f)));
      }

      SECTION("min cv subtracts half rotation") {
        level_knob = 0.88f;
        level_cv = MIN_CV;
        upstage.step();
        REQUIRE_THAT(out, near(selected_range.scale(level_knob - 0.5f)));
      }

      SECTION("is not clamped") {
        level_knob = 1.f;
        level_cv = MAX_CV;
        upstage.step();
        REQUIRE_THAT(out, near(selected_range.scale(level_knob + 0.5f)));
      }
    }
  }
}
