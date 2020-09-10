#include "./fixtures/anchor-fixture.h"

#include "./fixtures/anchor-enums.h"

#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace cv_sequencer {
using dhe::unit::is_equal_to;
using dhe::unit::Suite;
using dhe::unit::TestRegistrar;

static inline void set_all_voltages(Module &module, float voltage) {
  module.input_ = voltage;
  module.output_ = voltage;
  for (int step = 0; step < step_count; step++) {
    module.start_level_[step] = voltage;
    module.end_level_[step] = voltage;
  }
}

class AnchorSuite : public Suite {
public:
  AnchorSuite() : Suite("dhe::cv_sequencer::Anchor") {}
  void register_tests(TestRegistrar add) override {
    add("AnchorType::Start: "
        "entered with AnchorSource::Level: "
        "voltage() with AnchorMode::Sample: "
        "is sampled level voltage",
        test(AnchorType::Start, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 0;
          auto constexpr default_entry_voltage = 3.343F;

          set_all_voltages(module, default_entry_voltage);
          auto constexpr level_entry_voltage = default_entry_voltage + 1.F;
          module.start_level_[step] = level_entry_voltage;
          module.start_source_[step] = AnchorSource::Level;
          anchor.enter(step);

          set_all_voltages(module, level_entry_voltage + 1.F);

          module.start_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            module.start_source_[step] = source;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(level_entry_voltage));
          }
        }));

    add("AnchorType::Start: "
        "entered with AnchorSource::In: "
        "voltage() with AnchorMode::Sample: "
        "is sampled IN voltage",
        test(AnchorType::Start, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 1;
          auto constexpr default_entry_voltage = 1.343F;

          set_all_voltages(module, default_entry_voltage);
          auto constexpr input_entry_voltage = default_entry_voltage + 1.F;
          module.input_ = input_entry_voltage;
          module.start_source_[step] = AnchorSource::In;
          anchor.enter(step);

          set_all_voltages(module, input_entry_voltage + 1.F);

          module.start_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            module.start_source_[step] = source;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(input_entry_voltage));
          }
        }));

    add("AnchorType::Start: "
        "entered with AnchorSource::Out: "
        "voltage() with AnchorMode::Sample: "
        "is sampled OUT voltage",
        test(AnchorType::Start, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 2;
          auto constexpr default_entry_voltage = 4.343F;

          set_all_voltages(module, default_entry_voltage);
          auto constexpr output_entry_voltage = default_entry_voltage + 1.F;
          module.output_ = output_entry_voltage;
          module.start_source_[step] = AnchorSource::Out;
          anchor.enter(step);

          set_all_voltages(module, output_entry_voltage + 1.F);

          module.start_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            module.start_source_[step] = source;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(output_entry_voltage));
          }
        }));

    add("AnchorType::Start: "
        "entered with AnchorSource::Aux: "
        "voltage() with AnchorMode::Sample: "
        "is sampled AUX voltage",
        test(AnchorType::Start, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 3;
          auto constexpr default_entry_voltage = 1.343F;

          set_all_voltages(module, default_entry_voltage);
          auto constexpr aux_entry_voltage = default_entry_voltage + 1.F;
          module.aux_ = aux_entry_voltage;
          module.start_source_[step] = AnchorSource::Aux;
          anchor.enter(step);

          set_all_voltages(module, aux_entry_voltage + 1.F);

          module.start_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            module.start_source_[step] = source;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(aux_entry_voltage));
          }
        }));

    add("AnchorType::Start: "
        "voltage() with AnchorMode::Track and AnchorSource::Level: "
        "is current level voltage",
        test(AnchorType::Start, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 4;
          auto constexpr default_entry_voltage = 6.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(module, default_entry_voltage);
            module.start_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            module.start_mode_[step] = AnchorMode::Track;
            module.start_source_[step] = AnchorSource::Level;
            auto constexpr current_level_voltage = default_entry_voltage + 1.F;
            module.start_level_[step] = current_level_voltage;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(current_level_voltage));
          }
        }));

    add("AnchorType::Start: "
        "voltage() with AnchorMode::Track and AnchorSource::In: "
        "is current IN voltage",
        test(AnchorType::Start, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 5;
          auto constexpr default_entry_voltage = 5.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(module, default_entry_voltage);
            module.start_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            module.start_mode_[step] = AnchorMode::Track;
            module.start_source_[step] = AnchorSource::In;
            auto constexpr current_input_voltage = default_entry_voltage + 1.F;
            module.input_ = current_input_voltage;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(current_input_voltage));
          }
        }));

    add("AnchorType::Start: "
        "voltage() with AnchorMode::Track and AnchorSource::Out: "
        "is current OUT voltage",
        test(AnchorType::Start, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 6;
          auto constexpr default_entry_voltage = 7.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(module, default_entry_voltage);
            module.start_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            module.start_mode_[step] = AnchorMode::Track;
            module.start_source_[step] = AnchorSource::Out;
            auto constexpr current_output_voltage = default_entry_voltage + 1.F;
            module.output_ = current_output_voltage;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(current_output_voltage));
          }
        }));

    add("AnchorType::Start: "
        "voltage() with AnchorMode::Track and AnchorSource::Aux: "
        "is current AUX voltage",
        test(AnchorType::Start, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 7;
          auto constexpr default_entry_voltage = 5.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(module, default_entry_voltage);
            module.start_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            module.start_mode_[step] = AnchorMode::Track;
            module.start_source_[step] = AnchorSource::Aux;
            auto constexpr current_aux_voltage = default_entry_voltage + 1.F;
            module.aux_ = current_aux_voltage;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(current_aux_voltage));
          }
        }));

    add("AnchorType::End: "
        "entered with AnchorSource::Level: "
        "voltage() with AnchorMode::Sample: "
        "is sampled level voltage",
        test(AnchorType::End, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 7;
          auto constexpr default_entry_voltage = 3.343F;

          set_all_voltages(module, default_entry_voltage);
          auto constexpr level_entry_voltage = default_entry_voltage + 1.F;
          module.end_level_[step] = level_entry_voltage;
          module.end_source_[step] = AnchorSource::Level;
          anchor.enter(step);

          set_all_voltages(module, level_entry_voltage + 1.F);

          module.start_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            module.end_source_[step] = source;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(level_entry_voltage));
          }
        }));

    add("AnchorType::End: "
        "entered with AnchorSource::In: "
        "voltage() with AnchorMode::Sample: "
        "is sampled IN voltage",
        test(AnchorType::End, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 6;
          auto constexpr default_entry_voltage = 4.343F;

          set_all_voltages(module, default_entry_voltage);
          auto constexpr input_entry_voltage = default_entry_voltage + 1.F;
          module.input_ = input_entry_voltage;
          module.end_source_[step] = AnchorSource::In;
          anchor.enter(step);

          set_all_voltages(module, input_entry_voltage + 1.F);

          module.start_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            module.end_source_[step] = source;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(input_entry_voltage));
          }
        }));

    add("AnchorType::End: "
        "entered with AnchorSource::Out: "
        "voltage() with AnchorMode::Sample: "
        "is sampled OUT voltage",
        test(AnchorType::End, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 5;
          auto constexpr default_entry_voltage = 4.343F;

          set_all_voltages(module, default_entry_voltage);
          auto constexpr output_entry_voltage = default_entry_voltage + 1.F;
          module.output_ = output_entry_voltage;
          module.end_source_[step] = AnchorSource::Out;
          anchor.enter(step);

          set_all_voltages(module, output_entry_voltage + 1.F);

          module.start_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            module.end_source_[step] = source;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(output_entry_voltage));
          }
        }));

    add("AnchorType::End: "
        "entered with AnchorSource::Aux: "
        "voltage() with AnchorMode::Sample: "
        "is sampled Aux voltage",
        test(AnchorType::End, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 4;
          auto constexpr default_entry_voltage = 1.343F;

          set_all_voltages(module, default_entry_voltage);
          auto constexpr aux_entry_voltage = default_entry_voltage + 1.F;
          module.aux_ = aux_entry_voltage;
          module.end_source_[step] = AnchorSource::Aux;
          anchor.enter(step);

          set_all_voltages(module, aux_entry_voltage + 1.F);

          module.start_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            module.end_source_[step] = source;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(aux_entry_voltage));
          }
        }));

    add("AnchorType::End: "
        "voltage() with AnchorMode::Track and AnchorSource::Level: "
        "is current level voltage",
        test(AnchorType::End, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 3;
          auto constexpr default_entry_voltage = 6.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(module, default_entry_voltage);
            module.end_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            module.end_mode_[step] = AnchorMode::Track;
            module.end_source_[step] = AnchorSource::Level;
            auto constexpr current_level_voltage = default_entry_voltage + 1.F;
            module.end_level_[step] = current_level_voltage;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(current_level_voltage));
          }
        }));

    add("AnchorType::End: "
        "voltage() with AnchorMode::Track and AnchorSource::In: "
        "is current IN voltage",
        test(AnchorType::End, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 2;
          auto constexpr default_entry_voltage = 5.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(module, default_entry_voltage);
            module.end_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            module.end_mode_[step] = AnchorMode::Track;
            module.end_source_[step] = AnchorSource::In;
            auto constexpr current_input_voltage = default_entry_voltage + 1.F;
            module.input_ = current_input_voltage;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(current_input_voltage));
          }
        }));

    add("AnchorType::End: "
        "voltage() with AnchorMode::Track and AnchorSource::Out: "
        "is current OUT voltage",
        test(AnchorType::End, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 1;
          auto constexpr default_entry_voltage = 7.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(module, default_entry_voltage);
            module.end_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            module.end_mode_[step] = AnchorMode::Track;
            module.end_source_[step] = AnchorSource::Out;
            auto constexpr current_output_voltage = default_entry_voltage + 1.F;
            module.output_ = current_output_voltage;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(current_output_voltage));
          }
        }));

    add("AnchorType::End: "
        "voltage() with AnchorMode::Track and AnchorSource::Aux: "
        "is current AUX voltage",
        test(AnchorType::End, [](Tester &t, Module &module, Anchor &anchor) {
          auto constexpr step = 0;
          auto constexpr default_entry_voltage = 7.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(module, default_entry_voltage);
            module.end_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            module.end_mode_[step] = AnchorMode::Track;
            module.end_source_[step] = AnchorSource::Aux;
            auto constexpr current_aux_voltage = default_entry_voltage + 1.F;
            module.aux_ = current_aux_voltage;
            t.assert_that(name_of(source), anchor.voltage(),
                          is_equal_to(current_aux_voltage));
          }
        }));
  }
};

__attribute__((unused)) static auto _ = AnchorSuite{};

} // namespace cv_sequencer
} // namespace test
