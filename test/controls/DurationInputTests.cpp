#include "modules/controls/DurationInputs.h"

#include <gtest/gtest.h>

namespace dhe {

struct DurationInputTest : public ::testing::Test {
  // tolerance = 6 decimal places
  static auto constexpr tolerance = 0.000001F;

  rack::engine::Param knob;
};

TEST_F(DurationInputTest, shortRange_minimumDuration_isShortRangeLowerBound) {
  knob.setValue(0.F);
  auto const expected = shortDurationRange.lowerBound();

  auto const duration = dhe::duration(knob, shortDurationRange);

  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationInputTest, shortRange_middleDuration_isOneTenthOfShortRangeUpperBound) {
  knob.setValue(0.5F);
  auto const expected = shortDurationRange.upperBound() * 0.1F;

  auto const duration = dhe::duration(knob, shortDurationRange);

  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationInputTest, shortRange_maximumDuration_isShortRangeUpperBound) {
  knob.setValue(1.F);
  auto const expected = shortDurationRange.upperBound();

  auto const duration = dhe::duration(knob, shortDurationRange);

  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationInputTest, mediumRange_minimumDuration_isMediumRangeLowerBound) {
  knob.setValue(0.F);
  auto const expected = mediumDurationRange.lowerBound();

  auto const duration = dhe::duration(knob, mediumDurationRange);

  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationInputTest, mediumRange_middleDuration_isOneTenthOfMediumRangeUpperBound) {
  knob.setValue(0.5F);
  auto const expected = mediumDurationRange.upperBound() * 0.1F;

  auto const duration = dhe::duration(knob, mediumDurationRange);

  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationInputTest, mediumRange_maximumDuration_isMediumRangeUpperBound) {
  knob.setValue(1.F);
  auto const expected = mediumDurationRange.upperBound();

  auto const duration = dhe::duration(knob, mediumDurationRange);

  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationInputTest, longRange_minimumDuration_isLongRangeLowerBound) {
  knob.setValue(0.F);
  auto const expected = longDurationRange.lowerBound();

  auto const duration = dhe::duration(knob, longDurationRange);

  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationInputTest, longRange_middleDuration_isOneTenthOfLongRangeUpperBound) {
  knob.setValue(0.5F);
  auto const expected = longDurationRange.upperBound() * 0.1F;

  auto const duration = dhe::duration(knob, longDurationRange);

  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationInputTest, longRange_maximumDuration_isLongRangeUpperBound) {
  knob.setValue(1.F);
  auto const expected = longDurationRange.upperBound();

  auto const duration = dhe::duration(knob, longDurationRange);

  ASSERT_NEAR(duration, expected, expected * tolerance);
}
} // namespace dhe
