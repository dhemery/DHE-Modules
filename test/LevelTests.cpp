
#include "modules/controls/LevelControl.h"

#include <cmath>
#include <gtest/gtest.h>

namespace {

struct LevelTest : public ::testing::Test {

  static constexpr float tolerance = 0.000001f;
  static constexpr float bipolar_range_switch_position = 0.f;
  static constexpr float unipolar_range_switch_position = 1.f;

  rack::engine::Param knob_param{};
  rack::engine::Param switch_param{};
  rack::engine::Input cv_input{};

  DHE::LevelControl level{knob_param, switch_param, cv_input};

  LevelTest() {
    cv_input.setVoltage(0.f);
  }
};

TEST_F(LevelTest, bipolarRange_minimumLevel_isBipolarRangeLowerBound) {
  switch_param.setValue(bipolar_range_switch_position);
  knob_param.setValue(0.f);

  const auto expected = DHE::LevelControl::bipolar_range.lower_bound;
  ASSERT_NEAR(level.voltage(), expected, std::abs(expected * tolerance));
}

TEST_F(LevelTest, bipolarRange_middleLevel_isMidpointOfBipolarRange) {
  switch_param.setValue(bipolar_range_switch_position);
  knob_param.setValue(0.5f);

  const auto expected = (DHE::LevelControl::bipolar_range.upper_bound -
                         DHE::LevelControl::bipolar_range.lower_bound) *
                            0.5 +
                        DHE::LevelControl::bipolar_range.lower_bound;
  ASSERT_NEAR(level.voltage(), expected, std::abs(expected * tolerance));
}

TEST_F(LevelTest, bipolarRange_maximumLevel_isBipolarRangeUpperBound) {
  switch_param.setValue(bipolar_range_switch_position);
  knob_param.setValue(1.f);

  const auto expected = DHE::LevelControl::bipolar_range.upper_bound;
  ASSERT_NEAR(level.voltage(), expected, std::abs(expected * tolerance));
}

TEST_F(LevelTest, unipolarRange_minimumLevel_isUnipolarRangeLowerBound) {
  switch_param.setValue(unipolar_range_switch_position);
  knob_param.setValue(0.f);

  const auto expected = DHE::LevelControl::unipolar_range.lower_bound;
  ASSERT_NEAR(level.voltage(), expected, expected * tolerance);
}

TEST_F(LevelTest, unipolarRange_middleLevel_isMidpointOfUnipolarRange) {
  switch_param.setValue(unipolar_range_switch_position);
  knob_param.setValue(0.5f);

  const auto expected = (DHE::LevelControl::unipolar_range.upper_bound -
                         DHE::LevelControl::unipolar_range.lower_bound) *
                            0.5 +
                        DHE::LevelControl::unipolar_range.lower_bound;
  ASSERT_NEAR(level.voltage(), expected, expected * tolerance);
}

TEST_F(LevelTest, unipolarRange_maximumLevel_isUnipolarRangeUpperBound) {
  switch_param.setValue(unipolar_range_switch_position);
  knob_param.setValue(1.f);

  const auto expected = DHE::LevelControl::unipolar_range.upper_bound;
  ASSERT_NEAR(level.voltage(), expected, expected * tolerance);
}

} // namespace
