#include "signals/shape-signals.h"

#include "dheunit/test.h"
#include "helpers/assertions.h"

#include <string>
#include <vector>

namespace test {
namespace shapes {
using dhe::JShape;
using dhe::SShape;
using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_near;

struct ShapeTest {
  float input;  // NOLINT
  float output; // NOLINT

  template <typename S> void run(Tester &t, float curvature) {
    auto const name = std::to_string(input) + " → " + std::to_string(output);
    t.run(name, [this, curvature](Tester &t) {
      auto const applied = S::apply(input, curvature);
      assert_that(t, "apply", applied, is_near(output, 0.000001F));
      auto const inverted = S::invert(applied, curvature);
      assert_that(t, "invert", inverted, is_near(input, 0.001F));
    });
  }
};

// Think of a taper as a shape with a fixed curvature
template <typename S> struct TaperSuite {
  std::string name;             // NOLINT
  float curvature;              // NOLINT
  std::vector<ShapeTest> tests; // NOLINT

  void run(Tester &t) {
    t.run(name, [this](Tester &t) {
      for (auto test : tests) {
        test.template run<S>(t, curvature);
      }
    });
  }
};

// Curvature 0 creates a linear taper for both J and S shapes
static auto zero_curvature_tests = std::vector<ShapeTest>{
    {0.F, 0.F},     {0.05F, 0.05F}, {0.45F, 0.45F}, {0.5F, 0.5F},
    {0.55F, 0.55F}, {0.95F, 0.95F}, {1.F, 1.F},
};

// Characterize each J taper with these inputs:
// 0.00 The lower bound of the input range
// 0.05 Just above the lower bound
// 0.50 The middle
// 0.95 Just below the upper bound
// 1.00 The upper bound
static auto j_shape_tests = std::vector<TaperSuite<JShape>>{
    {
        .name = "JShape::min-curvature",
        .curvature = -0.9999F,
        .tests =
            {
                // Slope is very steep near the low bound, very low elsewhere
                {0.F, 0.F},
                {0.05F, 0.999051F},
                {0.5F, 0.999950F},
                {0.95F, 0.999997F},
                {1.F, 1.F},
            },
    },
    {
        .name = "JShape::negative-curvature",
        .curvature = -0.8F,
        .tests =
            {
                // Slope increases slowly at first, then quickly
                {0.F, 0.F},
                {0.05F, 0.321429F},
                {0.5F, 0.9F},
                {0.95F, 0.994186F},
                {1.F, 1.F},
            },
    },
    {
        .name = "JShape::linear",
        .curvature = 0.F,
        .tests = zero_curvature_tests,
    },
    {
        .name = "JShape::positive-curvature",
        .curvature = 0.8F,
        .tests =
            {
                // Slope increases slowly at first, then quickly
                {0.F, 0.F},
                {0.05F, 0.00581395F},
                {0.5F, 0.1F},
                {0.95F, 0.678571F},
                {1.F, 1.F},
            },
    },
    {
        .name = "JShape::max-curvature",
        .curvature = 0.9999F,
        .tests =
            {
                // Slope is very steep near the high bound, very low elsewhere
                {0.F, 0.F},
                {0.05F, 2.63136e-06F},
                {0.5F, 5.00083e-05F},
                {0.95F, 0.000949243F},
                {1.F, 1.F},
            },
    },
};

// Characterize each S taper with these inputs:
// 0.00 The lower bound of the input range
// 0.05 Just above the lower bound
// 0.25 The middle of the lower half
// 0.45 Just below the middle
// 0.50 The middle
// 0.55 Just above the middle
// 0.75 The middle of the upper half
// 0.95 Just below the upper bound
// 1.00 The upper bound
static auto s_shape_tests = std::vector<TaperSuite<SShape>>{
    {
        .name = "SShape minimum curvature (-0.9999)",
        .curvature = -0.9999F,
        .tests =
            {
                // Slope is very high at the very ends, very low elsewhere
                {0.F, 0.F},
                {0.05F, 0.499775F},
                {0.25F, 0.499975F},
                {0.45F, 0.499997F},
                {0.5F, 0.5F},
                {0.55F, 0.500003F},
                {0.75F, 0.500025F},
                {0.95F, 0.500225F},
                {1.F, 1.F},
            },
    },
    {
        .name = "SShape negative curvature (-0.8)",
        .curvature = -0.8F,
        .tests =
            {
                // Slope is low in the center, high at ends
                {0.F, 0.F},
                {0.05F, 0.25F},
                {0.25F, 0.45F},
                {0.45F, 0.493902F},
                {0.5F, 0.5F},
                {0.55F, 0.506098F},
                {0.75F, 0.55F},
                {0.95F, 0.75F},
                {1.F, 1.F},
            },
    },
    {
        .name = "SShape zero curvature (linear)",
        .curvature = 0.F,
        .tests = zero_curvature_tests,
    },
    {
        .name = "SShape positive curvature (0.8)",
        .curvature = 0.8F,
        .tests =
            {
                // Slope is high in the center, low at ends
                {0.F, 0.F},
                {0.05F, 6.09756e-3F},
                {0.25F, 0.05F},
                {0.45F, 0.25F},
                {0.5F, 0.5F},
                {0.55F, 0.75F},
                {0.75F, 0.95F},
                {0.95F, 0.993902F},
                {1.F, 1.F},
            },
    },
    {
        .name = "SShape maximum curvature (0.9999)",
        .curvature = 0.9999F,
        .tests =
            {
                // Slope is very high in very center, very low elsewhere
                {0.F, 0.F},
                {0.05F, 2.77162e-6F},
                {0.25F, 2.50041e-5F},
                {0.45F, 2.24888e-4F},
                {0.5F, 0.5F},
                {0.55F, 0.999775F},
                {0.75F, 0.999975F},
                {0.95F, 0.999997F},
                {1.F, 1.F},
            },
    },
};

struct ShapeSuite : Suite {
  ShapeSuite() : Suite{"Shapes"} {}

  void run(Tester &t) override {
    for (auto &test : j_shape_tests) {
      test.run(t);
    }
    for (auto &test : s_shape_tests) {
      test.run(t);
    }
  }
};

static auto _ = ShapeSuite{};
} // namespace shapes
} // namespace test
