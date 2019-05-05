
#include "modules/controls/Level.h"

#include "gmock/gmock.h"
#include <cmath>

namespace {

using ::testing::AnyNumber;
using ::testing::AtMost;
using ::testing::Expectation;
using ::testing::NiceMock;
using ::testing::Return;

struct LevelTest : public ::testing::Test {

  static constexpr float tolerance = 0.000001f;
  static constexpr float bipolar_range_switch_position = 0.f;
  static constexpr float unipolar_range_switch_position = 1.f;

  rack::engine::Param knob_param{};
  rack::engine::Param switch_param{};
  rack::engine::Input cv_input{};

  DHE::Level level{};

  LevelTest() {
    cv_input.setVoltage(0.f);
    level.config(&knob_param, &switch_param, &cv_input);
  }
};

TEST_F(LevelTest, bipolarRange_minimumLevel_isBipolarRangeLowerBound) {
  switch_param.setValue(bipolar_range_switch_position);
  knob_param.setValue(0.f);

  const auto expected = DHE::Level::bipolar_range.lower_bound;
  ASSERT_NEAR(level(), expected, std::abs(expected * tolerance));
}

TEST_F(LevelTest, bipolarRange_middleLevel_isMidpointOfBipolarRange) {
  switch_param.setValue(bipolar_range_switch_position);
  knob_param.setValue(0.5f);

  const auto expected = (DHE::Level::bipolar_range.upper_bound -
                         DHE::Level::bipolar_range.lower_bound) *
                            0.5 +
                        DHE::Level::bipolar_range.lower_bound;
  ASSERT_NEAR(level(), expected, std::abs(expected * tolerance));
}

TEST_F(LevelTest, bipolarRange_maximumLevel_isBipolarRangeUpperBound) {
  switch_param.setValue(bipolar_range_switch_position);
  knob_param.setValue(1.f);

  const auto expected = DHE::Level::bipolar_range.upper_bound;
  ASSERT_NEAR(level(), expected, std::abs(expected * tolerance));
}

TEST_F(LevelTest, unipolarRange_minimumLevel_isUnipolarRangeLowerBound) {
  switch_param.setValue(unipolar_range_switch_position);
  knob_param.setValue(0.f);

  const auto expected = DHE::Level::unipolar_range.lower_bound;
  ASSERT_NEAR(level(), expected, expected * tolerance);
}

TEST_F(LevelTest, unipolarRange_middleLevel_isMidpointOfUnipolarRange) {
  switch_param.setValue(unipolar_range_switch_position);
  knob_param.setValue(0.5f);

  const auto expected = (DHE::Level::unipolar_range.upper_bound -
                         DHE::Level::unipolar_range.lower_bound) *
                            0.5 +
                        DHE::Level::unipolar_range.lower_bound;
  ASSERT_NEAR(level(), expected, expected * tolerance);
}

TEST_F(LevelTest, unipolarRange_maximumLevel_isUnipolarRangeUpperBound) {
  switch_param.setValue(unipolar_range_switch_position);
  knob_param.setValue(1.f);

  const auto expected = DHE::Level::unipolar_range.upper_bound;
  ASSERT_NEAR(level(), expected, expected * tolerance);
}

} // namespace
