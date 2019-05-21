#include "../include/modules/controls/DurationControl.h"

#include <gtest/gtest.h>

namespace DHE {

struct DurationTest : public ::testing::Test {
  // tolerance = 6 decimal places
  static constexpr float tolerance = 0.000001f;

  Range const *range;
  float rotation;

  std::function<float()> getRotation{[this]() -> float { return rotation; }};

  std::function<Range const *()> getRange{
      [this]() -> Range const * { return range; }};

  Duration::Control duration{getRotation, getRange};
};

TEST_F(DurationTest, shortRange_minimumDuration_isShortRangeLowerBound) {
  range = &Duration::shortRange;
  rotation = 0.f;

  const auto expected = Duration::shortRange.lower_bound;
  ASSERT_NEAR(duration.seconds(), expected, expected * tolerance);
}

TEST_F(DurationTest,
       shortRange_middleDuration_isOneTenthOfShortRangeUpperBound) {
  range = &Duration::shortRange;
  rotation = 0.5f;

  const auto expected = Duration::shortRange.upper_bound / 10.f;
  ASSERT_NEAR(duration.seconds(), expected, expected * tolerance);
}

TEST_F(DurationTest, shortRange_maximumDuration_isShortRangeUpperBound) {
  range = &Duration::shortRange;
  rotation = 1.f;

  const auto expected = Duration::shortRange.upper_bound;
  ASSERT_NEAR(duration.seconds(), expected, expected * tolerance);
}

TEST_F(DurationTest, mediumRange_minimumDuration_isMediumRangeLowerBound) {
  range = &Duration::mediumRange;
  rotation = 0.f;

  const auto expected = Duration::mediumRange.lower_bound;
  ASSERT_NEAR(duration.seconds(), expected, expected * tolerance);
}

TEST_F(DurationTest,
       mediumRange_middleDuration_isOneTenthOfMediumRangeUpperBound) {
  range = &Duration::mediumRange;
  rotation = 0.5f;

  const auto expected = Duration::mediumRange.upper_bound / 10.f;
  ASSERT_NEAR(duration.seconds(), expected, expected * tolerance);
}

TEST_F(DurationTest, mediumRange_maximumDuration_isMediumRangeUpperBound) {
  range = &Duration::mediumRange;
  rotation = 1.f;

  const auto expected = Duration::mediumRange.upper_bound;
  ASSERT_NEAR(duration.seconds(), expected, expected * tolerance);
}

TEST_F(DurationTest, longRange_minimumDuration_isLongRangeLowerBound) {
  range = &Duration::longRange;
  rotation = 0.f;

  const auto expected = Duration::longRange.lower_bound;
  ASSERT_NEAR(duration.seconds(), expected, expected * tolerance);
}

TEST_F(DurationTest, longRange_middleDuration_isOneTenthOfLongRangeUpperBound) {
  range = &Duration::longRange;
  rotation = 0.5f;

  const auto expected = Duration::longRange.upper_bound / 10.f;
  ASSERT_NEAR(duration.seconds(), expected, expected * tolerance);
}

TEST_F(DurationTest, longRange_maximumDuration_isLongRangeUpperBound) {
  range = &Duration::longRange;
  rotation = 1.f;

  const auto expected = Duration::longRange.upper_bound;
  ASSERT_NEAR(duration.seconds(), expected, expected * tolerance);
}
} // namespace DHE
