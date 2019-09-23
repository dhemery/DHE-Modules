
#include "modules/controls/LevelInputs.h"

#include <cmath>
#include <gtest/gtest.h>

namespace dhe {

struct LevelInputTest : public ::testing::Test {
  // tolerance = 6 decimal places
  static auto constexpr tolerance = 0.000001F;

  rack::engine::Param knob;
};

TEST_F(LevelInputTest, bipolarRange_minimumLevel_isBipolarRangeLowerBound) {
  knob.setValue(0.F);

  auto const level = dhe::level(knob, bipolarSignalRange);

  auto const expected = bipolarSignalRange.lowerBound();
  ASSERT_NEAR(level, expected, std::abs(expected * tolerance));
}

TEST_F(LevelInputTest, bipolarRange_middleLevel_isMidpointOfBipolarRange) {
  knob.setValue(0.5F);

  auto const level = dhe::level(knob, bipolarSignalRange);

  auto const expected = bipolarSignalRange.size() * 0.5 + bipolarSignalRange.lowerBound();
  ASSERT_NEAR(level, expected, std::abs(expected * tolerance));
}

TEST_F(LevelInputTest, bipolarRange_maximumLevel_isBipolarRangeUpperBound) {
  knob.setValue(1.F);

  auto const level = dhe::level(knob, bipolarSignalRange);

  auto const expected = bipolarSignalRange.upperBound();
  ASSERT_NEAR(level, expected, std::abs(expected * tolerance));
}

TEST_F(LevelInputTest, UnipolarRange_minimumLevel_isUnipolarRangeLowerBound) {
  knob.setValue(0.F);

  auto const level = dhe::level(knob, unipolarSignalRange);

  auto const expected = unipolarSignalRange.lowerBound();
  ASSERT_NEAR(level, expected, std::abs(expected * tolerance));
}

TEST_F(LevelInputTest, UnipolarRange_middleLevel_isMidpointOfUnipolarRange) {
  knob.setValue(0.5F);

  auto const level = dhe::level(knob, unipolarSignalRange);

  auto const expected = unipolarSignalRange.size() * 0.5 + unipolarSignalRange.lowerBound();
  ASSERT_NEAR(level, expected, std::abs(expected * tolerance));
}

TEST_F(LevelInputTest, UnipolarRange_maximumLevel_isUnipolarRangeUpperBound) {
  knob.setValue(1.F);

  auto const level = dhe::level(knob, unipolarSignalRange);

  auto const expected = unipolarSignalRange.upperBound();
  ASSERT_NEAR(level, expected, std::abs(expected * tolerance));
}

} // namespace dhe
