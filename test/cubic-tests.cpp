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

  auto input_gain = Scaled{input_gain_knob, DHE::CubicModule::gain_range()};
  auto output_gain = Scaled{output_gain_knob, DHE::CubicModule::gain_range()};

  x3_coefficient = 0.f;
  x2_coefficient = 0.f;
  x1_coefficient = 0.f;
  x0_coefficient = 0.f;

  input_gain = 1.f;
  output_gain = 1.f;

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
    input_signal = 1.234f;
    x1_coefficient = 1.f;

    SECTION("d=2 adds 10v") {
      x0_coefficient = 2.f;
      cubic.step();
      REQUIRE(output_signal==Approx(input_signal + 10.f));
    }

    SECTION("d=1 adds 5v") {
      x0_coefficient = 1.f;
      cubic.step();
      REQUIRE(output_signal==Approx(input_signal + 5.f));
    }

    SECTION("d=0 adds 0v") {
      x0_coefficient = 0.f;
      cubic.step();
      REQUIRE(output_signal==Approx(input_signal + 0.f));
    }

    SECTION("d=-1 subtracts 5v") {
      x0_coefficient = -1.f;
      cubic.step();

      REQUIRE(output_signal==Approx(input_signal - 5.f));
    }

    SECTION("d=-2 subtracts 10v") {
      x0_coefficient = -2.f;
      cubic.step();
      REQUIRE(output_signal==Approx(input_signal - 10.f));
    }

  }

  SECTION("input gain") {
    x3_coefficient = 1.340f;
    x2_coefficient = 0.482f;
    x1_coefficient = -1.929f;
    x0_coefficient = -0.283f;
    auto x = 0.849f;

    SECTION("0 gives f(0)") {
      input_gain = 0.f;
      input_signal = x*5.f;
      cubic.step();

      auto xg = x*input_gain();
      auto y = x3_coefficient()*xg*xg*xg + x2_coefficient()*xg*xg + x1_coefficient()*xg + x0_coefficient();
      REQUIRE(output_signal==Approx(y*5.f));
    }

    SECTION("1 gives f(x)") {
      input_gain = 1.f;
      input_signal = x*5.f;
      cubic.step();

      auto xg = x*input_gain();
      auto y = x3_coefficient()*xg*xg*xg + x2_coefficient()*xg*xg + x1_coefficient()*xg + x0_coefficient();
      REQUIRE(output_signal==Approx(y*5.f));
    }

    SECTION("2 gives f(2x)") {
      input_gain = 2.f;
      input_signal = x*5.f;
      cubic.step();

      auto xg = x*input_gain();
      auto y = x3_coefficient()*xg*xg*xg + x2_coefficient()*xg*xg + x1_coefficient()*xg + x0_coefficient();
      REQUIRE(output_signal==Approx(y*5.f));
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
