#include "modules/controls/DurationInputs.h"

#include <gtest/gtest.h>

namespace dhe {

struct DurationInputTest : public ::testing::Test {
  // tolerance = 6 decimal places
  static auto constexpr tolerance = 0.000001F;
  static auto constexpr durationKnob = 0;

  void SetUp() override { module.config(1, 1, 0); }

  void setParam(int param, float value) { module.params[param].setValue(value); }

  rack::engine::Module module{};
};

TEST_F(DurationInputTest, shortRange_minimumDuration_isShortRangeLowerBound) {
  setParam(durationKnob, 0.F);

  auto const duration = dhe::duration(&module, durationKnob, shortDurationRange);

  auto const expected = shortDurationRange.lowerBound();
  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationInputTest, shortRange_middleDuration_isOneTenthOfShortRangeUpperBound) {
  setParam(durationKnob, 0.5F);

  auto const duration = dhe::duration(&module, durationKnob, shortDurationRange);

  auto const expected = shortDurationRange.upperBound() * 0.1F;
  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationInputTest, shortRange_maximumDuration_isShortRangeUpperBound) {
  setParam(durationKnob, 1.F);

  auto const duration = dhe::duration(&module, durationKnob, shortDurationRange);

  auto const expected = shortDurationRange.upperBound();
  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationInputTest, mediumRange_minimumDuration_isMediumRangeLowerBound) {
  setParam(durationKnob, 0.F);

  auto const duration = dhe::duration(&module, durationKnob, mediumDurationRange);

  auto const expected = mediumDurationRange.lowerBound();
  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationInputTest, mediumRange_middleDuration_isOneTenthOfMediumRangeUpperBound) {
  setParam(durationKnob, 0.5F);

  auto const duration = dhe::duration(&module, durationKnob, mediumDurationRange);

  auto const expected = mediumDurationRange.upperBound() * 0.1F;
  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationInputTest, mediumRange_maximumDuration_isMediumRangeUpperBound) {
  setParam(durationKnob, 1.F);

  auto const duration = dhe::duration(&module, durationKnob, mediumDurationRange);

  auto const expected = mediumDurationRange.upperBound();
  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationInputTest, longRange_minimumDuration_isLongRangeLowerBound) {
  setParam(durationKnob, 0.F);

  auto const duration = dhe::duration(&module, durationKnob, longDurationRange);

  auto const expected = longDurationRange.lowerBound();
  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationInputTest, longRange_middleDuration_isOneTenthOfLongRangeUpperBound) {
  setParam(durationKnob, 0.5F);

  auto const duration = dhe::duration(&module, durationKnob, longDurationRange);

  auto const expected = longDurationRange.upperBound() * 0.1F;
  ASSERT_NEAR(duration, expected, expected * tolerance);
}

TEST_F(DurationInputTest, longRange_maximumDuration_isLongRangeUpperBound) {
  setParam(durationKnob, 1.F);

  auto const duration = dhe::duration(&module, durationKnob, longDurationRange);

  auto const expected = longDurationRange.upperBound();
  ASSERT_NEAR(duration, expected, expected * tolerance);
}
} // namespace dhe
