#include "components/sigmoid.h"

#include "dheunit/test.h"
#include "helpers/assertions.h"

#include <string>
#include <vector>

namespace test {
namespace components {
namespace sigmoid {

using dhe::JShape;
using dhe::SShape;
using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_near;

static auto constexpr tolerance = 0.000001F;

struct ShapeTest {
  float input;
  float output;

  template <typename S> void run(Tester &t, float curvature) {
    auto const name = std::string{"{"} + std::to_string(input) + "," +
                      std::to_string(output) + "}";
    t.run(name, [this, curvature](Tester &t) {
      auto const got = S::apply(input, curvature);
      assert_that(t, got, is_near(output, tolerance));
    });
  }
};

// Think of a taper as shape+curvature
template <typename S> struct TaperSuite {
  std::string name;
  float curvature;
  std::vector<ShapeTest> tests;

  void run(Tester &t) {
    t.run(name, [this](Tester &t) {
      for (auto test : tests) {
        test.template run<S>(t, curvature);
      }
    });
  }
};

// Characterize each taper by using as input:
// 0.00 The lower bound of the input range
// 0.05 Just above the lower bound
// 0.45 Just below the middle
// 0.50 The middle
// 0.55 Just above the middle
// 0.95 Just below the upper bound
// 1.00 The upper bound

// Curvature 0 creates a linear taper for both J and S shapes
static auto zero_curvature_tests = std::vector<ShapeTest>{
    {0.F, 0.F},     {0.05F, 0.05F}, {0.45F, 0.45F}, {0.5F, 0.5F},
    {0.55F, 0.55F}, {0.95F, 0.95F}, {1.F, 1.F},
};

static auto j_shape_tests = std::vector<TaperSuite<JShape>>{
    {
        .name = "JShape::min-curvature",
        .curvature = -0.9999F,
        .tests =
            {
                {0.F, 0.F},
                {0.05F, 0.999051F},
                {0.45F, 0.999939F},
                {0.5F, 0.999950F},
                {0.55F, 0.999959F},
                {0.95F, 0.999997F},
                {1.F, 1.F},
            },
    },
    {
        .name = "JShape::negative-curvature",
        .curvature = -0.8F,
        .tests =
            {
                {0.F, 0.F},
                {0.05F, 0.05F},
                {0.45F, 0.45F},
                {0.5F, 0.5F},
                {0.55F, 0.55F},
                {0.95F, 0.95F},
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
                {0.F, 0.F},
                {0.05F, 0.05F},
                {0.45F, 0.45F},
                {0.5F, 0.5F},
                {0.55F, 0.55F},
                {0.95F, 0.95F},
                {1.F, 1.F},
            },
    },
    {
        .name = "JShape::max-curvature",
        .curvature = 0.9999F,
        .tests =
            {
                {0.F, 0.F},
                {0.05F, 0.05F},
                {0.45F, 0.45F},
                {0.5F, 0.5F},
                {0.55F, 0.55F},
                {0.95F, 0.95F},
                {1.F, 1.F},
            },
    },
};

static auto s_shape_tests = std::vector<TaperSuite<SShape>>{
    {
        .name = "SShape minimum curvature (-0.9999)",
        .curvature = -0.9999F,
        .tests =
            {
                // Slope is very high at the very ends, very low elsewhere
                {0.F, 0.F},
                {0.05F, 0.499775F},
                {0.45F, 0.499997F},
                {0.5F, 0.5F},
                {0.55F, 0.500003F},
                {0.95F, 0.500225F},
                {1.F, 1.F},
            },
    },
    {
        .name = "SShape negative curvature (-0.8)",
        .curvature = -0.8F,
        .tests =
            {
                // Slope is llow in the center, high at ends
                {0.F, 0.F},
                {0.05F, 0.25F},
                {0.45F, 0.493902F},
                {0.5F, 0.5F},
                {0.55F, 0.506098F},
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
                {0.45F, 0.25F},
                {0.5F, 0.5F},
                {0.55F, 0.75F},
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
                {0.45F, 2.24888e-4F},
                {0.5F, 0.5F},
                {0.55F, 0.999775F},
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
} // namespace sigmoid
} // namespace components
} // namespace test
