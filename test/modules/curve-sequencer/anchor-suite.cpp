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
    add("after enter(s) with AnchorSource::In: "
        "voltage() called with AnchorMode:Snap: "
        "returns snapped IN voltage regardless of voltage at current source",
        test(AnchorType::Start,
             [](Tester &t, Module &controls, Anchor &anchor) {
               auto constexpr step = 2;
               controls.start_mode_[step] = AnchorMode::Snap;
               controls.start_source_[step] = AnchorSource::In;

               auto constexpr initial_input_voltage = 4.234F;
               controls.input_ = initial_input_voltage;

               anchor.enter(step);

               controls.input_ = initial_input_voltage + 1.F;
               t.assert_that("with new IN voltage", anchor.voltage(),
                             is_equal_to(initial_input_voltage));

               controls.start_source_[step] = AnchorSource::Level;
               t.assert_that("with AnchorSource::Level", anchor.voltage(),
                             is_equal_to(initial_input_voltage));

               controls.start_source_[step] = AnchorSource::Out;
               controls.output_ = 83.F;
               t.assert_that("with AnchorSource::Out", anchor.voltage(),
                             is_equal_to(initial_input_voltage));
             }));

    add("after enter(s) with AnchorSource::Level: "
        "voltage() called with AnchorMode:Snap: "
        "returns snapped start level voltage regardless of voltage at current "
        "source",
        test(AnchorType::Start,
             [](Tester &t, Module &controls, Anchor &anchor) {
               auto constexpr step = 3;
               controls.start_mode_[step] = AnchorMode::Snap;
               controls.start_source_[step] = AnchorSource::Level;

               auto constexpr initial_level_voltage = 2.349F;
               controls.start_level_[step] = initial_level_voltage;

               anchor.enter(step);

               controls.start_level_[step] = initial_level_voltage + 1.F;
               t.assert_that("with new level voltage", anchor.voltage(),
                             is_equal_to(initial_level_voltage));

               controls.start_source_[step] = AnchorSource::In;
               t.assert_that("with AnchorSource::In", anchor.voltage(),
                             is_equal_to(initial_level_voltage));

               controls.start_source_[step] = AnchorSource::Out;
               controls.output_ = 83.F;
               t.assert_that("with AnchorSource::Out", anchor.voltage(),
                             is_equal_to(initial_level_voltage));
             }));

    add("after enter(s) with AnchorSource::Out: "
        "voltage() called with AnchorMode:Snap: "
        "returns snapped OUT voltage regardless of voltage at current source",
        test(AnchorType::Start,
             [](Tester &t, Module &controls, Anchor &anchor) {
               auto constexpr step = 4;
               controls.start_mode_[step] = AnchorMode::Snap;
               controls.start_source_[step] = AnchorSource::Out;

               auto constexpr initial_level_voltage = 2.349F;
               controls.output_ = initial_level_voltage;

               anchor.enter(step);

               controls.output_ = initial_level_voltage + 1.F;
               t.assert_that("with new OUT voltage", anchor.voltage(),
                             is_equal_to(initial_level_voltage));

               controls.start_source_[step] = AnchorSource::In;
               t.assert_that("with AnchorSource::In", anchor.voltage(),
                             is_equal_to(initial_level_voltage));

               controls.start_source_[step] = AnchorSource::Level;
               controls.output_ = 83.F;
               t.assert_that("with AnchorSource::Level", anchor.voltage(),
                             is_equal_to(initial_level_voltage));
             }));

    add("with AnchorMode::Track: "
        "with AnchorSource::In: "
        "voltage() returns IN voltage regardless of snapped voltage",
        test(AnchorType::Start,
             [](Tester &t, Module &controls, Anchor &anchor) {
               auto constexpr step = 5;
               auto constexpr snapped_voltage = 92.234F;

               controls.start_mode_[step] = AnchorMode::Track;

               // Snap IN voltage
               controls.start_source_[step] = AnchorSource::In;
               controls.input_ = snapped_voltage;
               anchor.enter(step);

               // Update IN voltage
               auto input_voltage = 2.349F;
               controls.input_ = input_voltage;

               controls.start_source_[step] = AnchorSource::In;
               t.assert_that("after snapping IN", anchor.voltage(),
                             is_equal_to(input_voltage));

               // Snap Level voltage
               controls.start_source_[step] = AnchorSource::Level;
               controls.start_level_[step] = snapped_voltage;
               anchor.enter(step);

               // Update IN voltage
               input_voltage += 1.F;
               controls.input_ = input_voltage;

               controls.start_source_[step] = AnchorSource::In;
               t.assert_that("after snapping Level", anchor.voltage(),
                             is_equal_to(input_voltage));

               // Snap OUT voltage
               controls.start_source_[step] = AnchorSource::Out;
               controls.start_level_[step] = snapped_voltage;
               anchor.enter(step);

               // Updte IN voltage
               input_voltage += 1.F;
               controls.input_ = input_voltage;

               controls.start_source_[step] = AnchorSource::In;
               t.assert_that("after snapping OUT", anchor.voltage(),
                             is_equal_to(input_voltage));
             }));

    add("with AnchorMode::Track: "
        "with AnchorSource::Level: "
        "voltage() returns step level voltage regardless of snapped voltage",
        test(AnchorType::Start,
             [](Tester &t, Module &controls, Anchor &anchor) {
               auto constexpr step = 5;
               auto constexpr snapped_voltage = 92.234F;
               auto level_voltage = 2.341F;

               controls.start_mode_[step] = AnchorMode::Track;

               // Snap IN voltage
               controls.start_source_[step] = AnchorSource::In;
               controls.input_ = snapped_voltage;
               anchor.enter(step);

               // Update Level Voltage
               level_voltage += 1.F;
               controls.start_level_[step] = level_voltage;

               controls.start_source_[step] = AnchorSource::Level;
               t.assert_that("after snapping IN", anchor.voltage(),
                             is_equal_to(level_voltage));

               // Snap Level voltage
               controls.start_source_[step] = AnchorSource::Level;
               controls.start_level_[step] = snapped_voltage;
               anchor.enter(step);

               // Update Level voltage
               level_voltage += 1.F;
               controls.start_level_[step] = level_voltage;

               controls.start_source_[step] = AnchorSource::Level;
               t.assert_that("after snapping Level", anchor.voltage(),
                             is_equal_to(level_voltage));

               // Snap OUT voltage
               controls.start_source_[step] = AnchorSource::Out;
               controls.start_level_[step] = snapped_voltage;
               anchor.enter(step);

               // Update Level voltage
               level_voltage += 1.F;
               controls.start_level_[step] = level_voltage;

               controls.start_source_[step] = AnchorSource::Level;
               t.assert_that("after snapping OUT", anchor.voltage(),
                             is_equal_to(level_voltage));
             }));

    add("with AnchorMode::Track: "
        "with AnchorSource::Out: "
        "voltage() returns OUT voltage regardless of snapped voltage",
        test(AnchorType::Start,
             [](Tester &t, Module &controls, Anchor &anchor) {
               auto constexpr step = 5;
               auto constexpr snapped_voltage = 92.234F;
               auto output_voltage = 2.341F;

               controls.start_mode_[step] = AnchorMode::Track;

               // Snap IN voltage
               controls.start_source_[step] = AnchorSource::In;
               controls.input_ = snapped_voltage;
               anchor.enter(step);

               // Update OUT Voltage
               output_voltage += 1.F;
               controls.output_ = output_voltage;

               controls.start_source_[step] = AnchorSource::Out;
               t.assert_that("after snapping IN", anchor.voltage(),
                             is_equal_to(output_voltage));

               // Snap Level voltage
               controls.start_source_[step] = AnchorSource::Level;
               controls.start_level_[step] = snapped_voltage;
               anchor.enter(step);

               // Update OUT Voltage
               output_voltage += 1.F;
               controls.output_ = output_voltage;

               controls.start_source_[step] = AnchorSource::Out;
               t.assert_that("after snapping Level", anchor.voltage(),
                             is_equal_to(output_voltage));

               // Snap OUT voltage
               controls.start_source_[step] = AnchorSource::Out;
               controls.output_ = snapped_voltage;
               anchor.enter(step);

               // Update OUT Voltage
               output_voltage += 1.F;
               controls.output_ = output_voltage;

               controls.start_source_[step] = AnchorSource::Out;
               t.assert_that("after snapping OUT", anchor.voltage(),
                             is_equal_to(output_voltage));
             }));
  }
};

__attribute__((unused)) static auto _ = AnchorSuite{};

} // namespace curve_sequencer
} // namespace test
