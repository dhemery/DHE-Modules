
#include "modules/controls/LevelControl.h"

#include <cmath>
#include <gtest/gtest.h>

namespace DHE {

struct LevelTest : public ::testing::Test {

  static constexpr float tolerance = 0.000001f;
  static constexpr float bipolar_range_switch_position = 0.f;
  static constexpr float unipolar_range_switch_position = 1.f;

  Param knob_param{};
  Param switch_param{};
  Input cv_input{};

  LevelControl level{knob_param, switch_param, cv_input};

  LevelTest() {
    cv_input.setVoltage(0.f);
  }
};

TEST_F(LevelTest, bipolarRange_minimumLevel_isBipolarRangeLowerBound) {
  switch_param.setValue(bipolar_range_switch_position);
  knob_param.setValue(0.f);

  const auto expected = LevelControl::bipolar_range.lower_bound;
  ASSERT_NEAR(level.voltage(), expected, std::abs(expected * tolerance));
}

TEST_F(LevelTest, bipolarRange_middleLevel_isMidpointOfBipolarRange) {
  switch_param.setValue(bipolar_range_switch_position);
  knob_param.setValue(0.5f);

  const auto expected = (LevelControl::bipolar_range.upper_bound -
                         LevelControl::bipolar_range.lower_bound) *
                            0.5 +
                        LevelControl::bipolar_range.lower_bound;
  ASSERT_NEAR(level.voltage(), expected, std::abs(expected * tolerance));
}

TEST_F(LevelTest, bipolarRange_maximumLevel_isBipolarRangeUpperBound) {
  switch_param.setValue(bipolar_range_switch_position);
  knob_param.setValue(1.f);

  const auto expected = LevelControl::bipolar_range.upper_bound;
  ASSERT_NEAR(level.voltage(), expected, std::abs(expected * tolerance));
}

TEST_F(LevelTest, unipolarRange_minimumLevel_isUnipolarRangeLowerBound) {
  switch_param.setValue(unipolar_range_switch_position);
  knob_param.setValue(0.f);

  const auto expected = LevelControl::unipolar_range.lower_bound;
  ASSERT_NEAR(level.voltage(), expected, expected * tolerance);
}

TEST_F(LevelTest, unipolarRange_middleLevel_isMidpointOfUnipolarRange) {
  switch_param.setValue(unipolar_range_switch_position);
  knob_param.setValue(0.5f);

  const auto expected = (LevelControl::unipolar_range.upper_bound -
                         LevelControl::unipolar_range.lower_bound) *
                            0.5 +
                        LevelControl::unipolar_range.lower_bound;
  ASSERT_NEAR(level.voltage(), expected, expected * tolerance);
}

TEST_F(LevelTest, unipolarRange_maximumLevel_isUnipolarRangeUpperBound) {
  switch_param.setValue(unipolar_range_switch_position);
  knob_param.setValue(1.f);

  const auto expected = LevelControl::unipolar_range.upper_bound;
  ASSERT_NEAR(level.voltage(), expected, expected * tolerance);
}

} // namespace
