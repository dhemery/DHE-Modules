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
  DHE::CubicModule module{};
  auto a = Scaled{module.params[module.A_KNOB].value, DHE::CubicModule::coefficient_range()};
  auto b = Scaled{module.params[module.B_KNOB].value, DHE::CubicModule::coefficient_range()};
  auto c = Scaled{module.params[module.C_KNOB].value, DHE::CubicModule::coefficient_range()};
  auto d = Scaled{module.params[module.D_KNOB].value, DHE::CubicModule::coefficient_range()};

  auto input_gain = Scaled{module.params[module.INPUT_GAIN_KNOB].value, DHE::CubicModule::gain_range()};
  auto output_gain = Scaled{module.params[module.OUTPUT_GAIN_KNOB].value, DHE::CubicModule::gain_range()};

  auto &x_signal = module.inputs[module.X].value;
  auto &y_signal = module.outputs[module.Y].value;

  a = 0.f;
  b = 0.f;
  c = 0.f;
  d = 0.f;

  input_gain = 1.f;
  output_gain = 1.f;

  SECTION("ax^3") {
    auto x = 0.893f;

    a = 1.392f;

    x_signal = x*5.f;
    module.step();

    auto y = a()*x*x*x;
    REQUIRE(y_signal==Approx(y*5.f));
  }

  SECTION("bx^2") {
    auto x = 0.893f;

    b = 1.392f;
    x_signal = x*5.f;
    module.step();

    auto y = b()*x*x;
    REQUIRE(y_signal==Approx(y*5.f));
  }

  SECTION("cx^1") {
    auto x = 0.893f;

    c = 1.392f;
    x_signal = x*5.f;
    module.step();

    auto y = c()*x;
    REQUIRE(y_signal==Approx(y*5.f));
  }

  SECTION("dx^0") {
    x_signal = 1.234f;
    c = 1.f;

    SECTION("d=2 adds 10v") {
      d = 2.f;
      module.step();
      REQUIRE(y_signal==Approx(x_signal + 10.f));
    }

    SECTION("d=1 adds 5v") {
      d = 1.f;
      module.step();
      REQUIRE(y_signal==Approx(x_signal + 5.f));
    }

    SECTION("d=0 adds 0v") {
      d = 0.f;
      module.step();
      REQUIRE(y_signal==Approx(x_signal + 0.f));
    }

    SECTION("d=-1 subtracts 5v") {
      d = -1.f;
      module.step();

      REQUIRE(y_signal==Approx(x_signal - 5.f));
    }

    SECTION("d=-2 subtracts 10v") {
      d = -2.f;
      module.step();
      REQUIRE(y_signal==Approx(x_signal - 10.f));
    }

  }

  SECTION("input gain") {
    a = 1.340f;
    b = 0.482f;
    c = -1.929f;
    d = -0.283f;
    auto x = 0.849f;

    SECTION("0 gives f(0)") {
      input_gain = 0.f;
      x_signal = x*5.f;
      module.step();

      auto xg = x*input_gain();
      auto y = a()*xg*xg*xg + b()*xg*xg + c()*xg + d();
      REQUIRE(y_signal==Approx(y*5.f));
    }

    SECTION("1 gives f(x)") {
      input_gain = 1.f;
      x_signal = x*5.f;
      module.step();

      auto xg = x*input_gain();
      auto y = a()*xg*xg*xg + b()*xg*xg + c()*xg + d();
      REQUIRE(y_signal==Approx(y*5.f));
    }

    SECTION("2 gives f(2x)") {
      input_gain = 2.f;
      x_signal = x*5.f;
      module.step();

      auto xg = x*input_gain();
      auto y = a()*xg*xg*xg + b()*xg*xg + c()*xg + d();
      REQUIRE(y_signal==Approx(y*5.f));
    }
  }

  SECTION("output gain") {
    a = 1.340;
    b = 0.482f;
    c = -1.929f;
    d = -0.283f;
    auto x = 0.849f;

    SECTION("0 gives 0f(x) == 0") {
      output_gain = 0.f;
      x_signal = x*5.f;
      module.step();

      auto y = output_gain()*(a()*x*x*x + b()*x*x + c()*x + d());
      REQUIRE(y_signal==Approx(y*5.f));
    }

    SECTION("1 gives f(x)") {
      output_gain = 1.f;
      x_signal = x*5.f;
      module.step();

      auto y = output_gain()*(a()*x*x*x + b()*x*x + c()*x + d());
      REQUIRE(y_signal==Approx(y*5.f));
    }

    SECTION("2 gives 2f(x)") {
      output_gain = 2.f;
      x_signal = x*5.f;
      module.step();

      auto y = output_gain()*(a()*x*x*x + b()*x*x + c()*x + d());
      REQUIRE(y_signal==Approx(y*5.f));
    }
  }
}