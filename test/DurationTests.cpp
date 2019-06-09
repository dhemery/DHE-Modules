#include "modules/controls/DurationInputs.h"

#include <gtest/gtest.h>

namespace dhe {

struct DurationTest : public ::testing::Test {
  // tolerance = 6 decimal places
  static constexpr float tolerance = 0.000001f;
  rack::engine::Module module{};
  DurationTest() { module.config(1, 1, 0); }

  void setRotation(float rotation) { module.params[0].setValue(rotation); }
};

TEST_F(DurationTest, shortRange_minimumDuration_isShortRangeLowerBound) {
  auto const range = shortDurationRange;
  setRotation(0.F);

  auto const duration = dhe::duration(&module, 0, range);

  auto const expected = range.lowerBound;
  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationTest, shortRange_middleDuration_isOneTenthOfShortRangeUpperBound) {
  auto const range = shortDurationRange;
  setRotation(0.5F);

  auto const duration = dhe::duration(&module, 0, range);

  auto const expected = range.upperBound * 0.1F;
  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationTest, shortRange_maximumDuration_isShortRangeUpperBound) {
  auto const range = shortDurationRange;
  setRotation(1.F);

  auto const duration = dhe::duration(&module, 0, range);

  auto const expected = range.upperBound;
  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationTest, mediumRange_minimumDuration_isMediumRangeLowerBound) {
  auto const range = mediumDurationRange;
  setRotation(0.F);

  auto const duration = dhe::duration(&module, 0, range);

  auto const expected = range.lowerBound;
  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationTest, mediumRange_middleDuration_isOneTenthOfMediumRangeUpperBound) {
  auto const range = mediumDurationRange;
  setRotation(0.5F);

  auto const duration = dhe::duration(&module, 0, range);

  auto const expected = range.upperBound * 0.1F;
  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationTest, mediumRange_maximumDuration_isMediumRangeUpperBound) {
  auto const range = mediumDurationRange;
  setRotation(1.F);

  auto const duration = dhe::duration(&module, 0, range);

  auto const expected = range.upperBound;
  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationTest, longRange_minimumDuration_isLongRangeLowerBound) {
  auto const range = longDurationRange;
  setRotation(0.F);

  auto const duration = dhe::duration(&module, 0, range);

  auto const expected = range.lowerBound;
  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationTest, longRange_middleDuration_isOneTenthOfLongRangeUpperBound) {
  auto const range = longDurationRange;
  setRotation(0.5F);

  auto const duration = dhe::duration(&module, 0, range);

  auto const expected = range.upperBound * 0.1F;
  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationTest, longRange_maximumDuration_isLongRangeUpperBound) {
  auto const range = longDurationRange;
  setRotation(1.F);

  auto const duration = dhe::duration(&module, 0, range);

  auto const expected = range.upperBound;
  ASSERT_NEAR(duration, expected, expected * tolerance);
}
} // namespace dhe
