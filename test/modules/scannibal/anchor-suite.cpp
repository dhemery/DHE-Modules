#include "./fixtures/anchor-fixture.h"

#include "./fixtures/anchor-enums.h"

#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace scannibal {
using dhe::unit::is_equal_to;
using dhe::unit::Suite;

static inline void set_all_voltages(Module &module, float voltage) {
  module.in_a_ = voltage;
  module.in_b_ = voltage;
  module.in_c_ = voltage;
  module.output_ = voltage;
  for (int step = 0; step < step_count; step++) {
    module.phase_0_level_[step] = voltage;
    module.phase_1_level_[step] = voltage;
  }
}

class AnchorSuite : public Suite {
public:
  AnchorSuite() : Suite("dhe::scannibal::Anchor") {}
  void run(Tester &t) override {
    t.run(
        "AnchorType::Phase0: "
        "entered with AnchorSource::Level: "
        "voltage() with AnchorMode::Sample: "
        "is sampled level voltage",
        test(AnchorType::Phase0, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 0;
          auto constexpr default_entry_voltage = 3.343F;

          set_all_voltages(module, default_entry_voltage);
          auto constexpr level_entry_voltage = default_entry_voltage + 1.F;
          module.phase_0_source_[step] = AnchorSource::Level;
          module.phase_0_level_[step] = level_entry_voltage;
          anchor.enter(step);

          set_all_voltages(module, level_entry_voltage + 1.F);

          module.phase_0_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            module.phase_0_source_[step] = source;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(level_entry_voltage));
          }
        }));

    t.run(
        "AnchorType::Phase0: "
        "entered with AnchorSource::InA: "
        "voltage() with AnchorMode::Sample: "
        "is sampled A voltage",
        test(AnchorType::Phase0, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 1;
          auto constexpr default_entry_voltage = 1.343F;

          set_all_voltages(module, default_entry_voltage);
          auto constexpr in_a_entry_voltage = default_entry_voltage + 1.F;
          module.in_a_ = in_a_entry_voltage;
          module.phase_0_source_[step] = AnchorSource::InA;
          anchor.enter(step);

          set_all_voltages(module, in_a_entry_voltage + 1.F);

          module.phase_0_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            module.phase_0_source_[step] = source;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(in_a_entry_voltage));
          }
        }));

    t.run(
        "AnchorType::Phase0: "
        "entered with AnchorSource::InB: "
        "voltage() with AnchorMode::Sample: "
        "is sampled B voltage",
        test(AnchorType::Phase0, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 1;
          auto constexpr default_entry_voltage = 1.343F;

          set_all_voltages(module, default_entry_voltage);
          auto constexpr in_b_entry_voltage = default_entry_voltage + 1.F;
          module.in_b_ = in_b_entry_voltage;
          module.phase_0_source_[step] = AnchorSource::InB;
          anchor.enter(step);

          set_all_voltages(module, in_b_entry_voltage + 1.F);

          module.phase_0_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            module.phase_0_source_[step] = source;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(in_b_entry_voltage));
          }
        }));
    t.run(
        "AnchorType::Phase0: "
        "entered with AnchorSource::InC: "
        "voltage() with AnchorMode::Sample: "
        "is sampled C voltage",
        test(AnchorType::Phase0, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 1;
          auto constexpr default_entry_voltage = 1.343F;

          set_all_voltages(module, default_entry_voltage);
          auto constexpr in_c_entry_voltage = default_entry_voltage + 1.F;
          module.in_c_ = in_c_entry_voltage;
          module.phase_0_source_[step] = AnchorSource::InC;
          anchor.enter(step);

          set_all_voltages(module, in_c_entry_voltage + 1.F);

          module.phase_0_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            module.phase_0_source_[step] = source;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(in_c_entry_voltage));
          }
        }));

    t.run(
        "AnchorType::Phase0: "
        "entered with AnchorSource::Out: "
        "voltage() with AnchorMode::Sample: "
        "is sampled OUT voltage",
        test(AnchorType::Phase0, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 2;
          auto constexpr default_entry_voltage = 4.343F;

          set_all_voltages(module, default_entry_voltage);
          auto constexpr output_entry_voltage = default_entry_voltage + 1.F;
          module.output_ = output_entry_voltage;
          module.phase_0_source_[step] = AnchorSource::Out;
          anchor.enter(step);

          set_all_voltages(module, output_entry_voltage + 1.F);

          module.phase_0_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            module.phase_0_source_[step] = source;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(output_entry_voltage));
          }
        }));

    t.run(
        "AnchorType::Phase0: "
        "voltage() with AnchorMode::Track and AnchorSource::Level: "
        "is current level voltage",
        test(AnchorType::Phase0, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 4;
          auto constexpr default_entry_voltage = 6.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(module, default_entry_voltage);
            module.phase_0_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            module.phase_0_mode_[step] = AnchorMode::Track;
            module.phase_0_source_[step] = AnchorSource::Level;
            auto constexpr current_level_voltage = default_entry_voltage + 1.F;
            module.phase_0_level_[step] = current_level_voltage;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(current_level_voltage));
          }
        }));

    t.run(
        "AnchorType::Phase0: "
        "voltage() with AnchorMode::Track and AnchorSource::InA: "
        "is current A voltage",
        test(AnchorType::Phase0, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 5;
          auto constexpr default_entry_voltage = 5.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(module, default_entry_voltage);
            module.phase_0_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            module.phase_0_mode_[step] = AnchorMode::Track;
            module.phase_0_source_[step] = AnchorSource::InA;
            auto constexpr current_in_a_voltage = default_entry_voltage + 1.F;
            module.in_a_ = current_in_a_voltage;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(current_in_a_voltage));
          }
        }));

    t.run(
        "AnchorType::Phase0: "
        "voltage() with AnchorMode::Track and AnchorSource::InB: "
        "is current B voltage",
        test(AnchorType::Phase0, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 5;
          auto constexpr default_entry_voltage = 5.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(module, default_entry_voltage);
            module.phase_0_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            module.phase_0_mode_[step] = AnchorMode::Track;
            module.phase_0_source_[step] = AnchorSource::InB;
            auto constexpr current_in_b_voltage = default_entry_voltage + 1.F;
            module.in_b_ = current_in_b_voltage;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(current_in_b_voltage));
          }
        }));

    t.run(
        "AnchorType::Phase0: "
        "voltage() with AnchorMode::Track and AnchorSource::InC: "
        "is current C voltage",
        test(AnchorType::Phase0, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 5;
          auto constexpr default_entry_voltage = 5.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(module, default_entry_voltage);
            module.phase_0_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            module.phase_0_mode_[step] = AnchorMode::Track;
            module.phase_0_source_[step] = AnchorSource::InC;
            auto constexpr current_in_c_voltage = default_entry_voltage + 1.F;
            module.in_c_ = current_in_c_voltage;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(current_in_c_voltage));
          }
        }));

    t.run(
        "AnchorType::Phase0: "
        "voltage() with AnchorMode::Track and AnchorSource::Out: "
        "is current OUT voltage",
        test(AnchorType::Phase0, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 6;
          auto constexpr default_entry_voltage = 7.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(module, default_entry_voltage);
            module.phase_0_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            module.phase_0_mode_[step] = AnchorMode::Track;
            module.phase_0_source_[step] = AnchorSource::Out;
            auto constexpr current_output_voltage = default_entry_voltage + 1.F;
            module.output_ = current_output_voltage;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(current_output_voltage));
          }
        }));

    t.run(
        "AnchorType::Phase1: "
        "entered with AnchorSource::Level: "
        "voltage() with AnchorMode::Sample: "
        "is sampled level voltage",
        test(AnchorType::Phase1, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 7;
          auto constexpr default_entry_voltage = 3.343F;

          set_all_voltages(module, default_entry_voltage);
          auto constexpr level_entry_voltage = default_entry_voltage + 1.F;
          module.phase_1_source_[step] = AnchorSource::Level;
          module.phase_1_level_[step] = level_entry_voltage;
          anchor.enter(step);

          set_all_voltages(module, level_entry_voltage + 1.F);

          module.phase_0_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            module.phase_1_source_[step] = source;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(level_entry_voltage));
          }
        }));

    t.run(
        "AnchorType::Phase1: "
        "entered with AnchorSource::InA: "
        "voltage() with AnchorMode::Sample: "
        "is sampled A voltage",
        test(AnchorType::Phase1, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 6;
          auto constexpr default_entry_voltage = 4.343F;

          set_all_voltages(module, default_entry_voltage);
          auto constexpr in_a_entry_voltage = default_entry_voltage + 1.F;
          module.in_a_ = in_a_entry_voltage;
          module.phase_1_source_[step] = AnchorSource::InA;
          anchor.enter(step);

          set_all_voltages(module, in_a_entry_voltage + 1.F);

          module.phase_0_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            module.phase_1_source_[step] = source;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(in_a_entry_voltage));
          }
        }));

    t.run(
        "AnchorType::Phase1: "
        "entered with AnchorSource::InB: "
        "voltage() with AnchorMode::Sample: "
        "is sampled A voltage",
        test(AnchorType::Phase1, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 6;
          auto constexpr default_entry_voltage = 4.343F;

          set_all_voltages(module, default_entry_voltage);
          auto constexpr in_b_entry_voltage = default_entry_voltage + 1.F;
          module.in_b_ = in_b_entry_voltage;
          module.phase_1_source_[step] = AnchorSource::InB;
          anchor.enter(step);

          set_all_voltages(module, in_b_entry_voltage + 1.F);

          module.phase_0_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            module.phase_1_source_[step] = source;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(in_b_entry_voltage));
          }
        }));

    t.run(
        "AnchorType::Phase1: "
        "entered with AnchorSource::InC: "
        "voltage() with AnchorMode::Sample: "
        "is sampled C voltage",
        test(AnchorType::Phase1, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 6;
          auto constexpr default_entry_voltage = 4.343F;

          set_all_voltages(module, default_entry_voltage);
          auto constexpr in_c_entry_voltage = default_entry_voltage + 1.F;
          module.in_c_ = in_c_entry_voltage;
          module.phase_1_source_[step] = AnchorSource::InC;
          anchor.enter(step);

          set_all_voltages(module, in_c_entry_voltage + 1.F);

          module.phase_0_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            module.phase_1_source_[step] = source;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(in_c_entry_voltage));
          }
        }));

    t.run(
        "AnchorType::Phase1: "
        "entered with AnchorSource::Out: "
        "voltage() with AnchorMode::Sample: "
        "is sampled OUT voltage",
        test(AnchorType::Phase1, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 5;
          auto constexpr default_entry_voltage = 4.343F;

          set_all_voltages(module, default_entry_voltage);
          auto constexpr output_entry_voltage = default_entry_voltage + 1.F;
          module.output_ = output_entry_voltage;
          module.phase_1_source_[step] = AnchorSource::Out;
          anchor.enter(step);

          set_all_voltages(module, output_entry_voltage + 1.F);

          module.phase_0_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            module.phase_1_source_[step] = source;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(output_entry_voltage));
          }
        }));

    t.run(
        "AnchorType::Phase1: "
        "voltage() with AnchorMode::Track and AnchorSource::Level: "
        "is current level voltage",
        test(AnchorType::Phase1, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 3;
          auto constexpr default_entry_voltage = 6.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(module, default_entry_voltage);
            module.phase_1_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            module.phase_1_mode_[step] = AnchorMode::Track;
            module.phase_1_source_[step] = AnchorSource::Level;
            auto constexpr current_level_voltage = default_entry_voltage + 1.F;
            module.phase_1_level_[step] = current_level_voltage;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(current_level_voltage));
          }
        }));

    t.run(
        "AnchorType::Phase1: "
        "voltage() with AnchorMode::Track and AnchorSource::InA: "
        "is current A voltage",
        test(AnchorType::Phase1, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 2;
          auto constexpr default_entry_voltage = 5.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(module, default_entry_voltage);
            module.phase_1_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            module.phase_1_mode_[step] = AnchorMode::Track;
            module.phase_1_source_[step] = AnchorSource::InA;
            auto constexpr current_in_a_voltage = default_entry_voltage + 1.F;
            module.in_a_ = current_in_a_voltage;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(current_in_a_voltage));
          }
        }));

    t.run(
        "AnchorType::Phase1: "
        "voltage() with AnchorMode::Track and AnchorSource::InB: "
        "is current B voltage",
        test(AnchorType::Phase1, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 2;
          auto constexpr default_entry_voltage = 5.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(module, default_entry_voltage);
            module.phase_1_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            module.phase_1_mode_[step] = AnchorMode::Track;
            module.phase_1_source_[step] = AnchorSource::InB;
            auto constexpr current_in_b_voltage = default_entry_voltage + 1.F;
            module.in_b_ = current_in_b_voltage;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(current_in_b_voltage));
          }
        }));

    t.run(
        "AnchorType::Phase1: "
        "voltage() with AnchorMode::Track and AnchorSource::InC: "
        "is current C voltage",
        test(AnchorType::Phase1, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 2;
          auto constexpr default_entry_voltage = 5.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(module, default_entry_voltage);
            module.phase_1_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            module.phase_1_mode_[step] = AnchorMode::Track;
            module.phase_1_source_[step] = AnchorSource::InC;
            auto constexpr current_in_c_voltage = default_entry_voltage + 1.F;
            module.in_c_ = current_in_c_voltage;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(current_in_c_voltage));
          }
        }));

    t.run(
        "AnchorType::Phase1: "
        "voltage() with AnchorMode::Track and AnchorSource::Out: "
        "is current OUT voltage",
        test(AnchorType::Phase1, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 1;
          auto constexpr default_entry_voltage = 7.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(module, default_entry_voltage);
            module.phase_1_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            module.phase_1_mode_[step] = AnchorMode::Track;
            module.phase_1_source_[step] = AnchorSource::Out;
            auto constexpr current_output_voltage = default_entry_voltage + 1.F;
            module.output_ = current_output_voltage;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(current_output_voltage));
          }
        }));
  }
};

__attribute__((unused)) static auto _ = AnchorSuite{};

} // namespace scannibal
} // namespace test
