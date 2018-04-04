#include <lib/catch.hpp>
#include <modules/cubic-module.h>

struct Scaled {
  Scaled(float &normalized, const DHE::Range &range) :
      normalized{normalized},
      range{range} {}

  float operator()() const {
    return range.scale(normalized);
  }

  void operator=(const float coefficient) {
    normalized = range.normalize(coefficient);
  }

  float &normalized;
  const DHE::Range &range;
};

TEST_CASE("Cubic Module") {
  DHE::CubicModule cubic{};
  auto &x3_knob = cubic.params[cubic.X3_KNOB].value;
  auto &x2_knob = cubic.params[cubic.X2_KNOB].value;
  auto &x1_knob = cubic.params[cubic.X1_KNOB].value;
  auto &x0_knob = cubic.params[cubic.X0_KNOB].value;
  auto &input_gain_knob = cubic.params[cubic.INPUT_GAIN_KNOB].value;
  auto &output_gain_knob = cubic.params[cubic.OUTPUT_GAIN_KNOB].value;
  auto &input_signal = cubic.inputs[cubic.IN].value;
  auto &output_signal = cubic.outputs[cubic.OUT].value;

  auto x3_coefficient = Scaled{x3_knob, DHE::CubicModule::coefficient_range()};
  auto x2_coefficient = Scaled{x2_knob, DHE::CubicModule::coefficient_range()};
  auto x1_coefficient = Scaled{x1_knob, DHE::CubicModule::coefficient_range()};
  auto x0_coefficient = Scaled{x0_knob, DHE::CubicModule::coefficient_range()};

  auto output_gain = Scaled{output_gain_knob, DHE::CubicModule::gain_range()};

  x3_knob = 0.5f; // 0x^3
  x2_knob = 0.5f; // 0x^2
  x1_knob = 0.5f; // 0x^1
  x0_knob = 0.5f; // 0x^0
  input_gain_knob = 0.5f; // input gain = 1
  output_gain_knob = 0.5f; // output gain = 1

  SECTION("ax^3") {
    auto x = -0.5f; // Arbitrary-ish

    SECTION("0% rotation -> -2x^3") {
      x3_knob = 0.f;
      auto y = -2.f*x*x*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("25% rotation -> -1x^3") {
      x3_knob = 0.25f;
      auto y = -1.f*x*x*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("50% rotation -> 0x^3") {
      x3_knob = 0.5f;
      auto y = 0.f*x*x*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("75% rotation -> 1x^3") {
      x3_knob = 0.75f;
      auto y = 1.f*x*x*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("100% rotation -> 2x^3") {
      x3_knob = 1.f;
      auto y = 2.f*x*x*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }
  }

  SECTION("bx^2") {
    auto x = -0.5f; // Arbitrary-ish

    SECTION("0% rotation -> -2x^2") {
      x2_knob = 0.f;
      auto y = -2.f*x*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("25% rotation -> -1x^2") {
      x2_knob = 0.25f;
      auto y = -1.f*x*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("50% rotation -> 0x^2") {
      x2_knob = 0.5f;
      auto y = 0.f*x*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("75% rotation -> 1x^2") {
      x2_knob = 0.75f;
      auto y = 1.f*x*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("100% rotation -> 2x^2") {
      x2_knob = 1.f;
      auto y = 2.f*x*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }
  }

  SECTION("cx^1") {
    auto x = -0.5f; // Arbitrary-ish

    SECTION("0% rotation -> -2x") {
      x1_knob = 0.f;
      auto y = -2.f*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("25% rotation -> -1x") {
      x1_knob = 0.25f;
      auto y = -1.f*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("50% rotation -> 0x") {
      x1_knob = 0.5f;
      auto y = 0.f*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("75% rotation -> 1x") {
      x1_knob = 0.75f;
      auto y = 1.f*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("100% rotation -> 2x") {
      x1_knob = 1.f;
      auto y = 2.f*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }
  }

  SECTION("dx^0") {
    auto x = -0.5f; // Arbitrary-ish

    SECTION("0% rotation -> -2") {
      x0_knob = 0.f;
      auto y = -2.f;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("25% rotation -> -1") {
      x0_knob = 0.25f;
      auto y = -1.f;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("50% rotation -> 0") {
      x0_knob = 0.5f;
      auto y = 0.f;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("75% rotation -> 1") {
      x0_knob = 0.75f;
      auto y = 1.f;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("100% rotation -> 2") {
      x0_knob = 1.f;
      auto y = 2.f;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }
  }

  SECTION("input gain") {
    auto x = -0.5f; // Arbitrary-ish
    // Arbitrary f(x): y = -2x^3 - x^2 + x + 2
    auto a = -2.f;
    auto b = -1.f;
    auto c = 1.f;
    auto d = 2.f;
    // Dial in f(x)
    x3_knob = 0.f;    // -2x^3
    x2_knob = 0.25f;  // -x^2
    x1_knob = 0.75f;  // x
    x0_knob = 1.f;    // 2


    SECTION("0% rotation -> f(0x)") {
      input_gain_knob = 0.f;
      auto input_gain = 0.f;
      auto xg = x*input_gain;
      auto y = a*xg*xg*xg + b*xg*xg + c*xg + d;

      input_signal = x*5.f;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("50% rotation -> f(1x)") {
      input_gain_knob = 0.f;
      auto input_gain = 0.f;
      auto xg = x*input_gain;
      auto y = a*xg*xg*xg + b*xg*xg + c*xg + d;

      input_signal = x*5.f;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("100% rotation -> f(2x)") {
      input_gain_knob = 0.f;
      auto input_gain = 0.f;
      auto xg = x*input_gain;
      auto y = a*xg*xg*xg + b*xg*xg + c*xg + d;

      input_signal = x*5.f;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }
  }

  SECTION("output gain") {
    x3_coefficient = 1.340;
    x2_coefficient = 0.482f;
    x1_coefficient = -1.929f;
    x0_coefficient = -0.283f;
    auto x = 0.849f;

    SECTION("0 gives 0f(x) == 0") {
      output_gain = 0.f;
      input_signal = x*5.f;
      cubic.step();

      auto y = output_gain()*(x3_coefficient()*x*x*x + x2_coefficient()*x*x + x1_coefficient()*x + x0_coefficient());
      REQUIRE(output_signal==Approx(y*5.f));
    }

    SECTION("1 gives f(x)") {
      output_gain = 1.f;
      input_signal = x*5.f;
      cubic.step();

      auto y = output_gain()*(x3_coefficient()*x*x*x + x2_coefficient()*x*x + x1_coefficient()*x + x0_coefficient());
      REQUIRE(output_signal==Approx(y*5.f));
    }

    SECTION("2 gives 2f(x)") {
      output_gain = 2.f;
      input_signal = x*5.f;
      cubic.step();

      auto y = output_gain()*(x3_coefficient()*x*x*x + x2_coefficient()*x*x + x1_coefficient()*x + x0_coefficient());
      REQUIRE(output_signal==Approx(y*5.f));
    }
  }
}
