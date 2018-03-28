#include <catch/catch.hpp>
#include <modules/upstage-module.h>

static const auto BUTTON_ON = 1;
static const auto BUTTON_OFF = 0;
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

  SECTION("LEVEL") {
    SECTION("UNI") {
      level_switch = LEVEL_SWITCH_UNI;

      SECTION("max rotation yields max unipolar signal") {
        level_knob = 1.f;
        upstage.step();
        REQUIRE_THAT(out, near(DHE::UNIPOLAR_SIGNAL_RANGE.upper_bound));
      }

      SECTION("min rotation yields min unipolar signal") {
        level_knob = 0.f;
        upstage.step();
        REQUIRE_THAT(out, near(DHE::UNIPOLAR_SIGNAL_RANGE.lower_bound));
      }

      SECTION("center rotation yields central unipolar signal ") {
        level_knob = 0.5f;
        upstage.step();
        REQUIRE_THAT(out, near(DHE::UNIPOLAR_SIGNAL_RANGE.scale(0.5f)));
      }
    }

    SECTION("BI") {
      level_switch = LEVEL_SWITCH_BI;

      SECTION("max rotation yields max bipolar signal") {
        level_knob = 1.f;
        upstage.step();
        REQUIRE_THAT(out, near(DHE::BIPOLAR_SIGNAL_RANGE.upper_bound));
      }

      SECTION("min rotation yields min bipolar signal") {
        level_knob = 0.f;
        upstage.step();
        REQUIRE_THAT(out, near(DHE::BIPOLAR_SIGNAL_RANGE.lower_bound));
      }

      SECTION("center rotation yields central bipolar signal") {
        level_knob = 0.5f;
        upstage.step();
        REQUIRE_THAT(out, near(DHE::BIPOLAR_SIGNAL_RANGE.scale(0.5f)));
      }
    }

    SECTION("CV") {
      auto &level_cv = upstage.inputs[upstage.LEVEL_CV].value;
      level_switch = LEVEL_SWITCH_UNI;

      SECTION("max cv adds 50% rotation") {
        level_knob = 0.35f;
        level_cv = MAX_CV;
        upstage.step();
        REQUIRE_THAT(out, near(DHE::UNIPOLAR_SIGNAL_RANGE.scale(level_knob + 0.5f)));
      }

      SECTION("min cv subtracts 50% rotation") {
        level_knob = 0.88f;
        level_cv = MIN_CV;
        upstage.step();
        REQUIRE_THAT(out, near(DHE::UNIPOLAR_SIGNAL_RANGE.scale(level_knob - 0.5f)));
      }

      SECTION("modulated value is not clamped") {
        level_knob = 1.f;
        level_cv = MAX_CV;
        upstage.step();
        REQUIRE_THAT(out, near(DHE::UNIPOLAR_SIGNAL_RANGE.scale(level_knob + 0.5f)));
      }
    }
  }

  SECTION("TRIG output") {
    auto &trig_in = upstage.inputs[upstage.TRIG_IN].value;
    auto &trig_out = upstage.outputs[upstage.TRIG_OUT].value;
    auto &trig_button = upstage.params[upstage.TRIG_BUTTON].value;
    trig_in = 0.f;

    SECTION("with TRIG button off") {
      trig_button = BUTTON_OFF;

      SECTION("sends TRIG input") {
        trig_in = 1e-4f;
        upstage.step();
        REQUIRE(trig_out==trig_in);

        trig_in = -1e-4f;
        upstage.step();
        REQUIRE(trig_out==trig_in);

        trig_in = 1e+4f;
        upstage.step();
        REQUIRE(trig_out==trig_in);

        trig_in = -1e+4f;
        upstage.step();
        REQUIRE(trig_out==trig_in);
      }
    }

    SECTION("with TRIG button on") {
      trig_button = BUTTON_ON;

      SECTION("sends 10v regardless of TRIG IN") {
        trig_in = 9.9f;
        upstage.step();
        REQUIRE(trig_out==10.f);

        trig_in = 1000.f;
        upstage.step();
        REQUIRE(trig_out==10.f);
      }
    }
  }

  SECTION("WAIT") {
    auto &trig_in = upstage.inputs[upstage.TRIG_IN].value;
    auto &trig_out = upstage.outputs[upstage.TRIG_OUT].value;
    auto &trig_button = upstage.params[upstage.TRIG_BUTTON].value;
    auto &wait_button = upstage.params[upstage.WAIT_BUTTON].value;
    auto &wait_in = upstage.inputs[upstage.WAIT_IN].value;

    trig_button = BUTTON_OFF;
    trig_in = 0.f;
    wait_button = BUTTON_OFF;
    wait_in = 0.f;

    SECTION("input gate high") {
      wait_in = 10.f;

      SECTION("suppresses TRIG input") {
        trig_in = 10.f;
        upstage.step();
        REQUIRE(trig_out == 0.f);
      }

      SECTION("suppresses TRIG button") {
        trig_button = BUTTON_ON;
        upstage.step();
        REQUIRE(trig_out == 0.f);
      }
    }

    SECTION("button on") {
      wait_button = BUTTON_ON;

      SECTION("suppresses TRIG input") {
        trig_in = 10.f;
        upstage.step();
        REQUIRE(trig_out == 0.f);
      }

      SECTION("suppresses TRIG button") {
        trig_button = BUTTON_ON;
        upstage.step();
        REQUIRE(trig_out == 0.f);
      }
    }
  }
}
