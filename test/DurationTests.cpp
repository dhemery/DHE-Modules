//#include "modules/controls/Duration.h"
//
//#include <gtest/gtest.h>
//
//namespace dhe {
//
//struct DurationTest : public ::testing::Test {
//  // tolerance = 6 decimal places
//  static constexpr float tolerance = 0.000001f;
//
//  Range const *range;
//  float rotation;
//
//  std::function<float()> getRotation{[this]() -> float { return rotation; }};
//
//  std::function<Range const *()> getRange{
//      [this]() -> Range const * { return range; }};
//
////  std::function<float()> duration = Duration::from(getRotation, getRange);
//  std::function<float()> duration = []() -> float { return 0.f; };
//};
//
//TEST_F(DurationTest, shortRange_minimumDuration_isShortRangeLowerBound) {
//  range = &duration::shortRange;
//  rotation = 0.f;
//
//  const auto expected = duration::shortRange.lower_bound;
//  ASSERT_NEAR(duration(), expected, expected * tolerance);
//}
//
//TEST_F(DurationTest,
//       shortRange_middleDuration_isOneTenthOfShortRangeUpperBound) {
//  range = &duration::shortRange;
//  rotation = 0.5f;
//
//  const auto expected = duration::shortRange.upper_bound / 10.f;
//  ASSERT_NEAR(duration(), expected, expected * tolerance);
//}
//
//TEST_F(DurationTest, shortRange_maximumDuration_isShortRangeUpperBound) {
//  range = &duration::shortRange;
//  rotation = 1.f;
//
//  const auto expected = duration::shortRange.upper_bound;
//  ASSERT_NEAR(duration(), expected, expected * tolerance);
//}
//
//TEST_F(DurationTest, mediumRange_minimumDuration_isMediumRangeLowerBound) {
//  range = &duration::mediumRange;
//  rotation = 0.f;
//
//  const auto expected = duration::mediumRange.lower_bound;
//  ASSERT_NEAR(duration(), expected, expected * tolerance);
//}
//
//TEST_F(DurationTest,
//       mediumRange_middleDuration_isOneTenthOfMediumRangeUpperBound) {
//  range = &duration::mediumRange;
//  rotation = 0.5f;
//
//  const auto expected = duration::mediumRange.upper_bound / 10.f;
//  ASSERT_NEAR(duration(), expected, expected * tolerance);
//}
//
//TEST_F(DurationTest, mediumRange_maximumDuration_isMediumRangeUpperBound) {
//  range = &duration::mediumRange;
//  rotation = 1.f;
//
//  const auto expected = duration::mediumRange.upper_bound;
//  ASSERT_NEAR(duration(), expected, expected * tolerance);
//}
//
//TEST_F(DurationTest, longRange_minimumDuration_isLongRangeLowerBound) {
//  range = &duration::longRange;
//  rotation = 0.f;
//
//  const auto expected = duration::longRange.lower_bound;
//  ASSERT_NEAR(duration(), expected, expected * tolerance);
//}
//
//TEST_F(DurationTest, longRange_middleDuration_isOneTenthOfLongRangeUpperBound) {
//  range = &duration::longRange;
//  rotation = 0.5f;
//
//  const auto expected = duration::longRange.upper_bound / 10.f;
//  ASSERT_NEAR(duration(), expected, expected * tolerance);
//}
//
//TEST_F(DurationTest, longRange_maximumDuration_isLongRangeUpperBound) {
//  range = &duration::longRange;
//  rotation = 1.f;
//
//  const auto expected = duration::longRange.upper_bound;
//  ASSERT_NEAR(duration(), expected, expected * tolerance);
//}
//} // namespace DHE
