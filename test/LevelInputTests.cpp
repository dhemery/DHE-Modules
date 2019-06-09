
#include "modules/controls/LevelInputs.h"

#include <cmath>
#include <gtest/gtest.h>

namespace dhe {

struct LevelInputTest : public ::testing::Test {
  // tolerance = 6 decimal places
  static auto constexpr tolerance = 0.000001F;
  static auto constexpr levelKnob = 0;

  void SetUp() override { module.config(1, 1, 0); }

  void setParam(int param, float value) { module.params[param].setValue(value); }

  rack::engine::Module module{};
};

TEST_F(LevelInputTest, bipolarRange_minimumLevel_isBipolarRangeLowerBound) {
  setParam(levelKnob, 0.F);

  auto const level = dhe::level(&module, levelKnob, bipolarSignalRange);

  auto const expected = bipolarSignalRange.lowerBound();
  ASSERT_NEAR(level, expected, std::abs(expected * tolerance));
}

TEST_F(LevelInputTest, bipolarRange_middleLevel_isMidpointOfBipolarRange) {
  setParam(levelKnob, 0.5F);

  auto const level = dhe::level(&module, levelKnob, bipolarSignalRange);

  auto const expected = bipolarSignalRange.size() * 0.5 + bipolarSignalRange.lowerBound();
  ASSERT_NEAR(level, expected, std::abs(expected * tolerance));
}

TEST_F(LevelInputTest, bipolarRange_maximumLevel_isBipolarRangeUpperBound) {
  setParam(levelKnob, 1.F);

  auto const level = dhe::level(&module, levelKnob, bipolarSignalRange);

  auto const expected = bipolarSignalRange.upperBound();
  ASSERT_NEAR(level, expected, std::abs(expected * tolerance));
}

TEST_F(LevelInputTest, UnipolarRange_minimumLevel_isUnipolarRangeLowerBound) {
  setParam(levelKnob, 0.F);

  auto const level = dhe::level(&module, levelKnob, unipolarSignalRange);

  auto const expected = unipolarSignalRange.lowerBound();
  ASSERT_NEAR(level, expected, std::abs(expected * tolerance));
}

TEST_F(LevelInputTest, UnipolarRange_middleLevel_isMidpointOfUnipolarRange) {
  setParam(levelKnob, 0.5F);

  auto const level = dhe::level(&module, levelKnob, unipolarSignalRange);

  auto const expected = unipolarSignalRange.size() * 0.5 + unipolarSignalRange.lowerBound();
  ASSERT_NEAR(level, expected, std::abs(expected * tolerance));
}

TEST_F(LevelInputTest, UnipolarRange_maximumLevel_isUnipolarRangeUpperBound) {
  setParam(levelKnob, 1.F);

  auto const level = dhe::level(&module, levelKnob, unipolarSignalRange);

  auto const expected = unipolarSignalRange.upperBound();
  ASSERT_NEAR(level, expected, std::abs(expected * tolerance));
}

} // namespace dhe
