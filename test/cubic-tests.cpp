#include <lib/catch.hpp>
#include <modules/cubic-module.h>

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

  auto &x3_cv = cubic.inputs[cubic.A_CV].value;
  auto &x2_cv = cubic.inputs[cubic.B_CV].value;
  auto &x1_cv = cubic.inputs[cubic.C_CV].value;
  auto &x0_cv = cubic.inputs[cubic.D_CV].value;
  auto &input_gain_cv = cubic.inputs[cubic.INPUT_GAIN_CV].value;
  auto &output_gain_cv = cubic.inputs[cubic.OUTPUT_GAIN_CV].value;

  x3_knob = 0.5f; // 0x^3
  x2_knob = 0.5f; // 0x^2
  x1_knob = 0.5f; // 0x^1
  x0_knob = 0.5f; // 0x^0
  input_gain_knob = 0.5f; // input gain = 1
  output_gain_knob = 0.5f; // output gain = 1

  input_gain_cv = 0.f;  // no modulation
  output_gain_cv = 0.f; // no modulation

  SECTION("ax^3") {
    auto x = -0.5f; // Arbitrary-ish

    SECTION("0% rotation -> -2x^3") {
      x3_knob = 0.f;
      auto x3_coefficient = -2.f;
      auto y = x3_coefficient*x*x*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("25% rotation -> -1x^3") {
      x3_knob = 0.25f;
      auto x3_coefficient = -1.f;
      auto y = x3_coefficient*x*x*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("50% rotation -> 0x^3") {
      x3_knob = 0.5f;
      auto x3_coefficient = 0.f;
      auto y = x3_coefficient*x*x*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("75% rotation -> 1x^3") {
      x3_knob = 0.75f;
      auto x3_coefficient = 1.f;
      auto y = x3_coefficient*x*x*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("100% rotation -> 2x^3") {
      x3_knob = 1.f;
      auto x3_coefficient = 2.f;
      auto y = x3_coefficient*x*x*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("CV") {
      SECTION("-5V subtracts 2 from coefficient") {
        x3_knob = 0.75f; // coefficient = 1
        x3_cv = -5.f;
        auto x3_coefficient = -1.f;
        auto y = x3_coefficient*x*x*x;

        input_signal = 5.f*x;
        cubic.step();

        REQUIRE(output_signal==5.f*y);
      }

      SECTION("0V leaves coefficient unchanged") {
        x3_knob = 0.75f; // coefficient = 1
        x3_cv = 0.f;
        auto x3_coefficient = 1.f;
        auto y = x3_coefficient*x*x*x;

        input_signal = 5.f*x;
        cubic.step();

        REQUIRE(output_signal==5.f*y);
      }

      SECTION("5V adds 2 to coefficient") {
        x3_knob = 0.25f; // coefficient = -1
        x3_cv = 5.f;
        auto x3_coefficient = 1.f;
        auto y = x3_coefficient*x*x*x;

        input_signal = 5.f*x;
        cubic.step();

        REQUIRE(output_signal==5.f*y);
      }
    }
  }

  SECTION("bx^2") {
    auto x = -0.5f; // Arbitrary-ish

    SECTION("0% rotation -> -2x^2") {
      x2_knob = 0.f;
      auto x2_coefficient = -2.f;
      auto y = x2_coefficient*x*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("25% rotation -> -1x^2") {
      x2_knob = 0.25f;
      auto x2_coefficient = -1.f;
      auto y = x2_coefficient*x*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("50% rotation -> 0x^2") {
      x2_knob = 0.5f;
      auto x2_coefficient = 0.f;
      auto y = x2_coefficient*x*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("75% rotation -> 1x^2") {
      x2_knob = 0.75f;
      auto x2_coefficient = 1.f;
      auto y = x2_coefficient*x*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("100% rotation -> 2x^2") {
      x2_knob = 1.f;
      auto x2_coefficient = 2.f;
      auto y = x2_coefficient*x*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("CV") {
      SECTION("-5V subtracts 2 from coefficient") {
        x2_knob = 0.75f; // coefficient = 1
        x2_cv = -5.f;
        auto x2_coefficient = -1.f;
        auto y = x2_coefficient*x*x;

        input_signal = 5.f*x;
        cubic.step();

        REQUIRE(output_signal==5.f*y);
      }

      SECTION("0V leaves coefficient unchanged") {
        x2_knob = 0.75f; // coefficient = 1
        x2_cv = 0.f;
        auto x2_coefficient = 1.f;
        auto y = x2_coefficient*x*x;

        input_signal = 5.f*x;
        cubic.step();

        REQUIRE(output_signal==5.f*y);
      }

      SECTION("5V adds 2 to coefficient") {
        x2_knob = 0.25f; // coefficient = -1
        x2_cv = 5.f;
        auto x2_coefficient = 1.f;
        auto y = x2_coefficient*x*x;

        input_signal = 5.f*x;
        cubic.step();

        REQUIRE(output_signal==5.f*y);
      }
    }
  }

  SECTION("cx^1") {
    auto x = -0.5f; // Arbitrary-ish

    SECTION("0% rotation -> -2x") {
      x1_knob = 0.f;
      auto x1_coefficient = -2.f;
      auto y = x1_coefficient*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("25% rotation -> -1x") {
      x1_knob = 0.25f;
      auto x1_coefficient = -1.f;
      auto y = x1_coefficient*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("50% rotation -> 0x") {
      x1_knob = 0.5f;
      auto x1_coefficient = 0.f;
      auto y = x1_coefficient*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("75% rotation -> 1x") {
      x1_knob = 0.75f;
      auto x1_coefficient = 1.f;
      auto y = x1_coefficient*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("100% rotation -> 2x") {
      x1_knob = 1.f;
      auto x1_coefficient = 2.f;
      auto y = x1_coefficient*x;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("CV") {
      SECTION("-5V subtracts 2 from coefficient") {
        x1_knob = 0.75f; // coefficient = 1
        x1_cv = -5.f;
        auto x1_coefficient = -1.f;
        auto y = x1_coefficient*x;

        input_signal = 5.f*x;
        cubic.step();

        REQUIRE(output_signal==5.f*y);
      }

      SECTION("0V leaves coefficient unchanged") {
        x1_knob = 0.75f; // coefficient = 1
        x1_cv = 0.f;
        auto x1_coefficient = 1.f;
        auto y = x1_coefficient*x;

        input_signal = 5.f*x;
        cubic.step();

        REQUIRE(output_signal==5.f*y);
      }

      SECTION("5V adds 2 to coefficient") {
        x1_knob = 0.25f; // coefficient = -1
        x1_cv = 5.f;
        auto x1_coefficient = 1.f;
        auto y = x1_coefficient*x;

        input_signal = 5.f*x;
        cubic.step();

        REQUIRE(output_signal==5.f*y);
      }
    }
  }

  SECTION("dx^0") {
    auto x = -0.5f; // Arbitrary-ish

    SECTION("0% rotation -> -2") {
      x0_knob = 0.f;
      auto x0_coefficient = -2.f;
      auto y = x0_coefficient;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("25% rotation -> -1") {
      x0_knob = 0.25f;
      auto x0_coefficient = -1.f;
      auto y = x0_coefficient;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("50% rotation -> 0") {
      x0_knob = 0.5f;
      auto x0_coefficient = 0.f;
      auto y = x0_coefficient;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("75% rotation -> 1") {
      x0_knob = 0.75f;
      auto x0_coefficient = 1.f;
      auto y = x0_coefficient;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("100% rotation -> 2") {
      x0_knob = 1.f;
      auto x0_coefficient = 2.f;
      auto y = x0_coefficient;

      input_signal = 5.f*x;
      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }
  }

  SECTION("input gain") {
    input_signal = -2.5f; // Arbitrary-ish
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
      auto x = input_signal*input_gain/5.f;
      auto y = a*x*x*x + b*x*x + c*x + d;

      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("50% rotation -> f(1x)") {
      input_gain_knob = 0.5f;
      auto input_gain = 1.f;
      auto x = input_signal*input_gain/5.f;
      auto y = a*x*x*x + b*x*x + c*x + d;

      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("100% rotation -> f(2x)") {
      input_gain_knob = 1.f;
      auto input_gain = 2.f;
      auto x = input_signal*input_gain/5.f;
      auto y = a*x*x*x + b*x*x + c*x + d;

      cubic.step();

      REQUIRE(output_signal==5.f*y);
    }

    SECTION("CV") {
      SECTION("-5V subtracts 1 from input gain") {
        input_gain_knob = 0.75f; // input gain = 1.5
        input_gain_cv = -5.f;
        auto modulated_input_gain = 0.5f;
        auto x = input_signal*modulated_input_gain/5.f;
        auto y = a*x*x*x + b*x*x + c*x + d;

        cubic.step();

        REQUIRE(output_signal==5.f*y);
      }

      SECTION("0V leaves input gain unaffected") {
        input_gain_knob = 0.5f; // input gain = 1
        input_gain_cv = 0.f;
        auto modulated_input_gain = 1.f;
        auto x = input_signal*modulated_input_gain/5.f;
        auto y = a*x*x*x + b*x*x + c*x + d;

        cubic.step();

        REQUIRE(output_signal==5.f*y);
      }

      SECTION("5V adds 1 to input gain") {
        input_gain_knob = 0.25f; // input gain = .5
        input_gain_cv = 5.f;
        auto modulated_input_gain = 1.5f;
        auto x = input_signal*modulated_input_gain/5.f;
        auto y = a*x*x*x + b*x*x + c*x + d;

        cubic.step();

        REQUIRE(output_signal==5.f*y);
      }
    }
  }

  SECTION("output gain") {
    auto x = -2.5f; // Arbitrary-ish
    // Arbitrary f(x): y = 2x^3 + x^2 - x - 2
    auto a = 2.f;
    auto b = 1.f;
    auto c = -1.f;
    auto d = -2.f;
    // Dial in f(x)
    x3_knob = 1.f;    // 2x^3
    x2_knob = 0.75f;  // x^2
    x1_knob = 0.25f;  // -x
    x0_knob = 0.f;    // -2


    SECTION("0% rotation -> 0f(x)") {
      output_gain_knob = 0.f;
      auto output_gain = 0.f;
      auto y = a*x*x*x + b*x*x + c*x + d;

      input_signal = x*5.f;
      cubic.step();

      REQUIRE(output_signal==5.f*output_gain*y);
    }

    SECTION("50% rotation -> 1f(x)") {
      output_gain_knob = 0.5f;
      auto output_gain = 1.f;
      auto y = a*x*x*x + b*x*x + c*x + d;

      input_signal = x*5.f;
      cubic.step();

      REQUIRE(output_signal==5.f*output_gain*y);
    }

    SECTION("100% rotation -> 2f(x)") {
      output_gain_knob = 1.f;
      auto output_gain = 2.f;
      auto y = a*x*x*x + b*x*x + c*x + d;

      input_signal = x*5.f;
      cubic.step();

      REQUIRE(output_signal==5.f*output_gain*y);
    }

    SECTION("CV") {
      SECTION("-5V subtracts 1 from output gain") {
        output_gain_knob = 0.75f; // output gain = 1.5
        output_gain_cv = -5.f;
        auto modulated_output_gain = 0.5f;
        auto y = a*x*x*x + b*x*x + c*x + d;

        input_signal = x*5.f;
        cubic.step();

        REQUIRE(output_signal==5.f*modulated_output_gain*y);
      }

      SECTION("0V leaves output gain unaffected") {
        output_gain_knob = 0.5f; // output gain = 1
        output_gain_cv = 0.f;
        auto modulated_output_gain = 1.f;
        auto y = a*x*x*x + b*x*x + c*x + d;

        input_signal = x*5.f;
        cubic.step();

        REQUIRE(output_signal==5.f*modulated_output_gain*y);
      }

      SECTION("5V adds 1 to output gain") {
        output_gain_knob = 0.25f; // output gain = .5
        output_gain_cv = 5.f;
        auto modulated_output_gain = 1.5f;
        auto y = a*x*x*x + b*x*x + c*x + d;

        input_signal = x*5.f;
        cubic.step();

        REQUIRE(output_signal==5.f*modulated_output_gain*y);
      }
    }
  }
}
