#include "modules/controls/DurationControl.h"

#include <gtest/gtest.h>

namespace DHE {

struct DurationTest : public ::testing::Test {

  // tolerance = 6 decimal places
  static constexpr float tolerance = 0.000001f;
  static constexpr float short_range_switch_position = 0.f;
  static constexpr float medium_range_switch_position = 1.f;
  static constexpr float long_range_switch_position = 2.f;

  Param knob_param{};
  Param switch_param{};
  Input cv_input{};

  DurationControl duration{knob_param, switch_param, cv_input};

  DurationTest() {
    cv_input.setVoltage(0.f);
  }
};

TEST_F(DurationTest, shortRange_minimumDuration_isShortRangeLowerBound) {
  switch_param.setValue(short_range_switch_position);
  knob_param.setValue(0.f);

  const auto expected = DurationControl::shortRange.lower_bound;
  ASSERT_NEAR(duration.seconds(), expected, expected * tolerance);
}

TEST_F(DurationTest, shortRange_middleDuration_isOneTenthOfShortRangeUpperBound) {
  switch_param.setValue(short_range_switch_position);
  knob_param.setValue(0.5f);

  const auto expected = DurationControl::shortRange.upper_bound / 10.f;
  ASSERT_NEAR(duration.seconds(), expected, expected * tolerance);
}

TEST_F(DurationTest, shortRange_maximumDuration_isShortRangeUpperBound) {
  switch_param.setValue(short_range_switch_position);
  knob_param.setValue(1.f);

  const auto expected = DurationControl::shortRange.upper_bound;
  ASSERT_NEAR(duration.seconds(), expected, expected * tolerance);
}

TEST_F(DurationTest, mediumRange_minimumDuration_isMediumRangeLowerBound) {
  switch_param.setValue(medium_range_switch_position);
  knob_param.setValue(0.f);

  const auto expected = DurationControl::mediumRange.lower_bound;
  ASSERT_NEAR(duration.seconds(), expected, expected * tolerance);
}

TEST_F(DurationTest, mediumRange_middleDuration_isOneTenthOfMediumRangeUpperBound) {
  switch_param.setValue(medium_range_switch_position);
  knob_param.setValue(0.5f);

  const auto expected = DurationControl::mediumRange.upper_bound / 10.f;
  ASSERT_NEAR(duration.seconds(), expected, expected * tolerance);
}

TEST_F(DurationTest, mediumRange_maximumDuration_isMediumRangeUpperBound) {
  switch_param.setValue(medium_range_switch_position);
  knob_param.setValue(1.f);

  const auto expected = DurationControl::mediumRange.upper_bound;
  ASSERT_NEAR(duration.seconds(), expected, expected * tolerance);
}

TEST_F(DurationTest, longRange_minimumDuration_isLongRangeLowerBound) {
  switch_param.setValue(long_range_switch_position);
  knob_param.setValue(0.f);

  const auto expected = DurationControl::longRange.lower_bound;
  ASSERT_NEAR(duration.seconds(), expected, expected * tolerance);
}

TEST_F(DurationTest, longRange_middleDuration_isOneTenthOfLongRangeUpperBound) {
  switch_param.setValue(long_range_switch_position);
  knob_param.setValue(0.5f);

  const auto expected = DurationControl::longRange.upper_bound / 10.f;
  ASSERT_NEAR(duration.seconds(), expected, expected * tolerance);
}

TEST_F(DurationTest, longRange_maximumDuration_isLongRangeUpperBound) {
  switch_param.setValue(long_range_switch_position);
  knob_param.setValue(1.f);

  const auto expected = DurationControl::longRange.upper_bound;
  ASSERT_NEAR(duration.seconds(), expected, expected * tolerance);
}
} // namespace
