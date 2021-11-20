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
  float normalized;
  float shaped;

  template <typename S> void run(Tester &t, float curvature) {
    auto const name = std::string{"{"} + std::to_string(normalized) + "," +
                      std::to_string(shaped) + "}";
    t.run(name, [this, curvature](Tester &t) {
      auto const applied = S::apply(normalized, curvature);
      assert_that(t, "apply()", applied, is_near(shaped, tolerance));

      auto const inverted = S::invert(shaped, curvature);
      assert_that(t, "invert()", inverted, is_near(normalized, tolerance));
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

// Curvature 0 is linear for both J and S shapes
static auto zero_curvature_tests = std::vector<ShapeTest>{
    {0.F, 0.F},     {0.05F, 0.05F}, {0.45F, 0.45F}, {0.5F, 0.5F},
    {0.55F, 0.55F}, {0.95F, 0.95F}, {1.F, 1.F},
};

static auto j_shape_tests = std::vector<TaperSuite<JShape>>{
    {
        .name = "JShape::linear",
        .curvature = 0.F,
        .tests = zero_curvature_tests,
    },
};

static auto s_shape_tests = std::vector<TaperSuite<SShape>>{
    {
        .name = "SShape::linear",
        .curvature = 0.F,
        .tests = zero_curvature_tests,
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
