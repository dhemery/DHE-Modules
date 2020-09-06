#include "./fixtures/anchor-fixture.h"
#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace curve_sequencer {
using dhe::unit::is_equal_to;
using dhe::unit::Suite;
using dhe::unit::TestRegistrar;

class AnchorSuite : public Suite {
public:
  AnchorSuite() : Suite("dhe::curve_sequencer::Anchor") {}
  void register_tests(TestRegistrar add) override {
    add("AnchorType::Start snapped AnchorSource::In: "
        "voltage() if AnchorMode:Snap: "
        "is snapped IN voltage regardless of voltage at current source",
        test(AnchorType::Start, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 2;
          module.start_mode_[step] = AnchorMode::Snap;
          module.start_source_[step] = AnchorSource::In;

          auto constexpr initial_input_voltage = 4.234F;
          module.input_ = initial_input_voltage;

          anchor.enter(step);

          module.input_ = initial_input_voltage + 1.F;
          t.assert_that("with new IN voltage", anchor.voltage(),
                        is_equal_to(initial_input_voltage));

          module.start_source_[step] = AnchorSource::Level;
          t.assert_that("with AnchorSource::Level", anchor.voltage(),
                        is_equal_to(initial_input_voltage));

          module.start_source_[step] = AnchorSource::Out;
          module.output_ = 83.F;
          t.assert_that("with AnchorSource::Out", anchor.voltage(),
                        is_equal_to(initial_input_voltage));
        }));

    add("AnchorType::Start snapped AnchorSource::Level: "
        "voltage() if AnchorMode:Snap: "
        "is snapped start level regardless of voltage at current source",
        test(AnchorType::Start, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 3;
          module.start_mode_[step] = AnchorMode::Snap;
          module.start_source_[step] = AnchorSource::Level;

          auto constexpr initial_level_voltage = 2.349F;
          module.start_level_[step] = initial_level_voltage;

          anchor.enter(step);

          module.start_level_[step] = initial_level_voltage + 1.F;
          t.assert_that("with new level voltage", anchor.voltage(),
                        is_equal_to(initial_level_voltage));

          module.start_source_[step] = AnchorSource::In;
          t.assert_that("with AnchorSource::In", anchor.voltage(),
                        is_equal_to(initial_level_voltage));

          module.start_source_[step] = AnchorSource::Out;
          module.output_ = 83.F;
          t.assert_that("with AnchorSource::Out", anchor.voltage(),
                        is_equal_to(initial_level_voltage));
        }));

    add("AnchorType::Start snapped AnchorSource::Out: "
        "voltage() if AnchorMode:Snap: "
        "is snapped OUT voltage regardless of voltage at current source",
        test(AnchorType::Start, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 4;
          module.start_mode_[step] = AnchorMode::Snap;
          module.start_source_[step] = AnchorSource::Out;

          auto constexpr initial_level_voltage = 2.349F;
          module.output_ = initial_level_voltage;

          anchor.enter(step);

          module.output_ = initial_level_voltage + 1.F;
          t.assert_that("with new OUT voltage", anchor.voltage(),
                        is_equal_to(initial_level_voltage));

          module.start_source_[step] = AnchorSource::In;
          t.assert_that("with AnchorSource::In", anchor.voltage(),
                        is_equal_to(initial_level_voltage));

          module.start_source_[step] = AnchorSource::Level;
          module.output_ = 83.F;
          t.assert_that("with AnchorSource::Level", anchor.voltage(),
                        is_equal_to(initial_level_voltage));
        }));

    add("AnchorType::Start: "
        "AnchorMode::Track: "
        "AnchorSource::In: "
        "voltage() is IN voltage regardless of snapped voltage",
        test(AnchorType::Start, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 5;
          auto constexpr snapped_voltage = 92.234F;

          module.start_mode_[step] = AnchorMode::Track;

          // Snap IN voltage
          module.start_source_[step] = AnchorSource::In;
          module.input_ = snapped_voltage;
          anchor.enter(step);

          // Update IN voltage
          auto input_voltage = 2.349F;
          module.input_ = input_voltage;

          module.start_source_[step] = AnchorSource::In;
          t.assert_that("after snapping IN", anchor.voltage(),
                        is_equal_to(input_voltage));

          // Snap Level voltage
          module.start_source_[step] = AnchorSource::Level;
          module.start_level_[step] = snapped_voltage;
          anchor.enter(step);

          // Update IN voltage
          input_voltage += 1.F;
          module.input_ = input_voltage;

          module.start_source_[step] = AnchorSource::In;
          t.assert_that("after snapping Level", anchor.voltage(),
                        is_equal_to(input_voltage));

          // Snap OUT voltage
          module.start_source_[step] = AnchorSource::Out;
          module.start_level_[step] = snapped_voltage;
          anchor.enter(step);

          // Updte IN voltage
          input_voltage += 1.F;
          module.input_ = input_voltage;

          module.start_source_[step] = AnchorSource::In;
          t.assert_that("after snapping OUT", anchor.voltage(),
                        is_equal_to(input_voltage));
        }));

    add("AnchorType::Start: "
        "AnchorMode::Track: "
        "AnchorSource::Level: "
        "voltage() is step level regardless of snapped voltage",
        test(AnchorType::Start, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 5;
          auto constexpr snapped_voltage = 92.234F;
          auto level_voltage = 2.341F;

          module.start_mode_[step] = AnchorMode::Track;

          // Snap IN voltage
          module.start_source_[step] = AnchorSource::In;
          module.input_ = snapped_voltage;
          anchor.enter(step);

          // Update Level Voltage
          level_voltage += 1.F;
          module.start_level_[step] = level_voltage;

          module.start_source_[step] = AnchorSource::Level;
          t.assert_that("after snapping IN", anchor.voltage(),
                        is_equal_to(level_voltage));

          // Snap Level voltage
          module.start_source_[step] = AnchorSource::Level;
          module.start_level_[step] = snapped_voltage;
          anchor.enter(step);

          // Update Level voltage
          level_voltage += 1.F;
          module.start_level_[step] = level_voltage;

          module.start_source_[step] = AnchorSource::Level;
          t.assert_that("after snapping Level", anchor.voltage(),
                        is_equal_to(level_voltage));

          // Snap OUT voltage
          module.start_source_[step] = AnchorSource::Out;
          module.start_level_[step] = snapped_voltage;
          anchor.enter(step);

          // Update Level voltage
          level_voltage += 1.F;
          module.start_level_[step] = level_voltage;

          module.start_source_[step] = AnchorSource::Level;
          t.assert_that("after snapping OUT", anchor.voltage(),
                        is_equal_to(level_voltage));
        }));

    add("AnchorType::Start: "
        "AnchorMode::Track: "
        "with AnchorSource::Out: "
        "voltage() is OUT voltage regardless of snapped voltage",
        test(AnchorType::Start, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 5;
          auto constexpr snapped_voltage = 92.234F;
          auto output_voltage = 2.341F;

          module.start_mode_[step] = AnchorMode::Track;

          // Snap IN voltage
          module.start_source_[step] = AnchorSource::In;
          module.input_ = snapped_voltage;
          anchor.enter(step);

          // Update OUT Voltage
          output_voltage += 1.F;
          module.output_ = output_voltage;

          module.start_source_[step] = AnchorSource::Out;
          t.assert_that("after snapping IN", anchor.voltage(),
                        is_equal_to(output_voltage));

          // Snap Level voltage
          module.start_source_[step] = AnchorSource::Level;
          module.start_level_[step] = snapped_voltage;
          anchor.enter(step);

          // Update OUT Voltage
          output_voltage += 1.F;
          module.output_ = output_voltage;

          module.start_source_[step] = AnchorSource::Out;
          t.assert_that("after snapping Level", anchor.voltage(),
                        is_equal_to(output_voltage));

          // Snap OUT voltage
          module.start_source_[step] = AnchorSource::Out;
          module.output_ = snapped_voltage;
          anchor.enter(step);

          // Update OUT Voltage
          output_voltage += 1.F;
          module.output_ = output_voltage;

          module.start_source_[step] = AnchorSource::Out;
          t.assert_that("after snapping OUT", anchor.voltage(),
                        is_equal_to(output_voltage));
        }));

    add("AnchorType::End snapped AnchorSource::In: "
        "voltage() if AnchorMode:Snap: "
        "is IN voltage regardless of voltage at current source",
        test(AnchorType::End, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 2;
          module.end_mode_[step] = AnchorMode::Snap;
          module.end_source_[step] = AnchorSource::In;

          auto constexpr initial_input_voltage = 4.234F;
          module.input_ = initial_input_voltage;

          anchor.enter(step);

          module.input_ = initial_input_voltage + 1.F;
          t.assert_that("with new IN voltage", anchor.voltage(),
                        is_equal_to(initial_input_voltage));

          module.end_source_[step] = AnchorSource::Level;
          t.assert_that("with AnchorSource::Level", anchor.voltage(),
                        is_equal_to(initial_input_voltage));

          module.end_source_[step] = AnchorSource::Out;
          module.output_ = 83.F;
          t.assert_that("with AnchorSource::Out", anchor.voltage(),
                        is_equal_to(initial_input_voltage));
        }));

    add("AnchorType::End snapped AnchorSource::Level: "
        "voltage() if AnchorMode:Snap: "
        "is snapped start level regardless of voltage at current source",
        test(AnchorType::End, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 3;
          module.end_mode_[step] = AnchorMode::Snap;
          module.end_source_[step] = AnchorSource::Level;

          auto constexpr initial_level_voltage = 2.349F;
          module.end_level_[step] = initial_level_voltage;

          anchor.enter(step);

          module.end_level_[step] = initial_level_voltage + 1.F;
          t.assert_that("with new level voltage", anchor.voltage(),
                        is_equal_to(initial_level_voltage));

          module.end_source_[step] = AnchorSource::In;
          t.assert_that("with AnchorSource::In", anchor.voltage(),
                        is_equal_to(initial_level_voltage));

          module.end_source_[step] = AnchorSource::Out;
          module.output_ = 83.F;
          t.assert_that("with AnchorSource::Out", anchor.voltage(),
                        is_equal_to(initial_level_voltage));
        }));

    add("AnchorType::End snapped AnchorSource::Out: "
        "voltage() if AnchorMode:Snap: "
        "returns snapped OUT voltage regardless of voltage at current source",
        test(AnchorType::End, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 4;
          module.end_mode_[step] = AnchorMode::Snap;
          module.end_source_[step] = AnchorSource::Out;

          auto constexpr initial_level_voltage = 2.349F;
          module.output_ = initial_level_voltage;

          anchor.enter(step);

          module.output_ = initial_level_voltage + 1.F;
          t.assert_that("with new OUT voltage", anchor.voltage(),
                        is_equal_to(initial_level_voltage));

          module.end_source_[step] = AnchorSource::In;
          t.assert_that("with AnchorSource::In", anchor.voltage(),
                        is_equal_to(initial_level_voltage));

          module.end_source_[step] = AnchorSource::Level;
          module.output_ = 83.F;
          t.assert_that("with AnchorSource::Level", anchor.voltage(),
                        is_equal_to(initial_level_voltage));
        }));

    add("AnchorType::End: "
        "AnchorMode::Track: "
        "AnchorSource::In: "
        "voltage() is IN voltage regardless of snapped voltage",
        test(AnchorType::End, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 5;
          auto constexpr snapped_voltage = 92.234F;

          module.end_mode_[step] = AnchorMode::Track;

          // Snap IN voltage
          module.end_source_[step] = AnchorSource::In;
          module.input_ = snapped_voltage;
          anchor.enter(step);

          // Update IN voltage
          auto input_voltage = 2.349F;
          module.input_ = input_voltage;

          module.end_source_[step] = AnchorSource::In;
          t.assert_that("after snapping IN", anchor.voltage(),
                        is_equal_to(input_voltage));

          // Snap Level voltage
          module.end_source_[step] = AnchorSource::Level;
          module.end_level_[step] = snapped_voltage;
          anchor.enter(step);

          // Update IN voltage
          input_voltage += 1.F;
          module.input_ = input_voltage;

          module.end_source_[step] = AnchorSource::In;
          t.assert_that("after snapping Level", anchor.voltage(),
                        is_equal_to(input_voltage));

          // Snap OUT voltage
          module.end_source_[step] = AnchorSource::Out;
          module.end_level_[step] = snapped_voltage;
          anchor.enter(step);

          // Updte IN voltage
          input_voltage += 1.F;
          module.input_ = input_voltage;

          module.end_source_[step] = AnchorSource::In;
          t.assert_that("after snapping OUT", anchor.voltage(),
                        is_equal_to(input_voltage));
        }));

    add("AnchorType::End: "
        "AnchorMode::Track: "
        "AnchorSource::Level: "
        "voltage() is step level regardless of snapped voltage",
        test(AnchorType::End, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 5;
          auto constexpr snapped_voltage = 92.234F;
          auto level_voltage = 2.341F;

          module.end_mode_[step] = AnchorMode::Track;

          // Snap IN voltage
          module.end_source_[step] = AnchorSource::In;
          module.input_ = snapped_voltage;
          anchor.enter(step);

          // Update Level Voltage
          level_voltage += 1.F;
          module.end_level_[step] = level_voltage;

          module.end_source_[step] = AnchorSource::Level;
          t.assert_that("after snapping IN", anchor.voltage(),
                        is_equal_to(level_voltage));

          // Snap Level voltage
          module.end_source_[step] = AnchorSource::Level;
          module.end_level_[step] = snapped_voltage;
          anchor.enter(step);

          // Update Level voltage
          level_voltage += 1.F;
          module.end_level_[step] = level_voltage;

          module.end_source_[step] = AnchorSource::Level;
          t.assert_that("after snapping Level", anchor.voltage(),
                        is_equal_to(level_voltage));

          // Snap OUT voltage
          module.end_source_[step] = AnchorSource::Out;
          module.end_level_[step] = snapped_voltage;
          anchor.enter(step);

          // Update Level voltage
          level_voltage += 1.F;
          module.end_level_[step] = level_voltage;

          module.end_source_[step] = AnchorSource::Level;
          t.assert_that("after snapping OUT", anchor.voltage(),
                        is_equal_to(level_voltage));
        }));

    add("AnchorType::End: "
        "AnchorMode::Track: "
        "AnchorSource::Out: "
        "voltage() is OUT voltage regardless of snapped voltage",
        test(AnchorType::End, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 5;
          auto constexpr snapped_voltage = 92.234F;
          auto output_voltage = 2.341F;

          module.end_mode_[step] = AnchorMode::Track;

          // Snap IN voltage
          module.end_source_[step] = AnchorSource::In;
          module.input_ = snapped_voltage;
          anchor.enter(step);

          // Update OUT Voltage
          output_voltage += 1.F;
          module.output_ = output_voltage;

          module.end_source_[step] = AnchorSource::Out;
          t.assert_that("after snapping IN", anchor.voltage(),
                        is_equal_to(output_voltage));

          // Snap Level voltage
          module.end_source_[step] = AnchorSource::Level;
          module.end_level_[step] = snapped_voltage;
          anchor.enter(step);

          // Update OUT Voltage
          output_voltage += 1.F;
          module.output_ = output_voltage;

          module.end_source_[step] = AnchorSource::Out;
          t.assert_that("after snapping Level", anchor.voltage(),
                        is_equal_to(output_voltage));

          // Snap OUT voltage
          module.end_source_[step] = AnchorSource::Out;
          module.output_ = snapped_voltage;
          anchor.enter(step);

          // Update OUT Voltage
          output_voltage += 1.F;
          module.output_ = output_voltage;

          module.end_source_[step] = AnchorSource::Out;
          t.assert_that("after snapping OUT", anchor.voltage(),
                        is_equal_to(output_voltage));
        }));
  }
};

__attribute__((unused)) static auto _ = AnchorSuite{};

} // namespace curve_sequencer
} // namespace test
