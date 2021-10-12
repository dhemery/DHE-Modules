#include "./fixtures/anchor-enums.h"
#include "./fixtures/anchor-fixture.h"

#include <dheunit/test.h>

namespace test {
namespace sequencizer {
using dhe::unit::Suite;

static inline void set_all_voltages(Signals &signals, float voltage) {
  signals.in_a_ = voltage;
  signals.in_b_ = voltage;
  signals.in_c_ = voltage;
  signals.output_ = voltage;
  for (int step = 0; step < step_count; step++) {
    signals.start_level_[step] = voltage;
    signals.end_level_[step] = voltage;
  }
}

class AnchorSuite : public Suite {
public:
  AnchorSuite() : Suite("dhe::sequencizer::Anchor") {}
  void run(Tester &t) override {
    t.run("AnchorType::Start: "
          "entered with AnchorSource::Level: "
          "voltage() with AnchorMode::Sample: "
          "is sampled level voltage",
          test(AnchorType::Start, [](Tester &t, Signals &signals,
                                     Anchor &anchor) {
            auto constexpr step = 0;
            auto constexpr default_entry_voltage = 3.343F;

            set_all_voltages(signals, default_entry_voltage);
            auto constexpr level_entry_voltage = default_entry_voltage + 1.F;
            signals.start_source_[step] = AnchorSource::Level;
            signals.start_level_[step] = level_entry_voltage;
            anchor.enter(step);

            set_all_voltages(signals, level_entry_voltage + 1.F);

            signals.start_mode_[step] = AnchorMode::Sample;
            for (auto const source : anchor_sources) {
              signals.start_source_[step] = source;
              auto const got = anchor.voltage();
              auto constexpr want = level_entry_voltage;
              if (got != want) {
                t.errorf("With source {} got anchor voltage {}, want {} ",
                         name_of(source), got, want);
              }
            }
          }));

    t.run("AnchorType::Start: "
          "entered with AnchorSource::InA: "
          "voltage() with AnchorMode::Sample: "
          "is sampled A voltage",
          test(AnchorType::Start, [](Tester &t, Signals &signals,
                                     Anchor &anchor) {
            auto constexpr step = 1;
            auto constexpr default_entry_voltage = 1.343F;

            set_all_voltages(signals, default_entry_voltage);
            auto constexpr in_a_entry_voltage = default_entry_voltage + 1.F;
            signals.in_a_ = in_a_entry_voltage;
            signals.start_source_[step] = AnchorSource::InA;
            anchor.enter(step);

            set_all_voltages(signals, in_a_entry_voltage + 1.F);

            signals.start_mode_[step] = AnchorMode::Sample;
            for (auto const source : anchor_sources) {
              signals.start_source_[step] = source;
              auto const got = anchor.voltage();
              auto constexpr want = in_a_entry_voltage;
              if (got != want) {
                t.errorf("With source {} got anchor voltage {}, want {} ",
                         name_of(source), got, want);
              }
            }
          }));

    t.run("AnchorType::Start: "
          "entered with AnchorSource::InB: "
          "voltage() with AnchorMode::Sample: "
          "is sampled B voltage",
          test(AnchorType::Start, [](Tester &t, Signals &signals,
                                     Anchor &anchor) {
            auto constexpr step = 1;
            auto constexpr default_entry_voltage = 1.343F;

            set_all_voltages(signals, default_entry_voltage);
            auto constexpr in_b_entry_voltage = default_entry_voltage + 1.F;
            signals.in_b_ = in_b_entry_voltage;
            signals.start_source_[step] = AnchorSource::InB;
            anchor.enter(step);

            set_all_voltages(signals, in_b_entry_voltage + 1.F);

            signals.start_mode_[step] = AnchorMode::Sample;
            for (auto const source : anchor_sources) {
              signals.start_source_[step] = source;
              auto const got = anchor.voltage();
              auto constexpr want = in_b_entry_voltage;
              if (got != want) {
                t.errorf("With source {} got anchor voltage {}, want {} ",
                         name_of(source), got, want);
              }
            }
          }));
    t.run("AnchorType::Start: "
          "entered with AnchorSource::InC: "
          "voltage() with AnchorMode::Sample: "
          "is sampled C voltage",
          test(AnchorType::Start, [](Tester &t, Signals &signals,
                                     Anchor &anchor) {
            auto constexpr step = 1;
            auto constexpr default_entry_voltage = 1.343F;

            set_all_voltages(signals, default_entry_voltage);
            auto constexpr in_c_entry_voltage = default_entry_voltage + 1.F;
            signals.in_c_ = in_c_entry_voltage;
            signals.start_source_[step] = AnchorSource::InC;
            anchor.enter(step);

            set_all_voltages(signals, in_c_entry_voltage + 1.F);

            signals.start_mode_[step] = AnchorMode::Sample;
            for (auto const source : anchor_sources) {
              signals.start_source_[step] = source;
              auto const got = anchor.voltage();
              auto constexpr want = in_c_entry_voltage;
              if (got != want) {
                t.errorf("With source {} got anchor voltage {}, want {} ",
                         name_of(source), got, want);
              }
            }
          }));

    t.run("AnchorType::Start: "
          "entered with AnchorSource::Out: "
          "voltage() with AnchorMode::Sample: "
          "is sampled OUT voltage",
          test(AnchorType::Start, [](Tester &t, Signals &signals,
                                     Anchor &anchor) {
            auto constexpr step = 2;
            auto constexpr default_entry_voltage = 4.343F;

            set_all_voltages(signals, default_entry_voltage);
            auto constexpr output_entry_voltage = default_entry_voltage + 1.F;
            signals.output_ = output_entry_voltage;
            signals.start_source_[step] = AnchorSource::Out;
            anchor.enter(step);

            set_all_voltages(signals, output_entry_voltage + 1.F);

            signals.start_mode_[step] = AnchorMode::Sample;
            for (auto const source : anchor_sources) {
              signals.start_source_[step] = source;
              auto const got = anchor.voltage();
              auto constexpr want = output_entry_voltage;
              if (got != want) {
                t.errorf("With source {} got anchor voltage {}, want {} ",
                         name_of(source), got, want);
              }
            }
          }));

    t.run("AnchorType::Start: "
          "voltage() with AnchorMode::Track and AnchorSource::Level: "
          "is current level voltage",
          test(AnchorType::Start,
               [](Tester &t, Signals &signals, Anchor &anchor) {
                 auto constexpr step = 4;
                 auto constexpr default_entry_voltage = 6.343F;

                 for (auto const source : anchor_sources) {
                   set_all_voltages(signals, default_entry_voltage);
                   signals.start_source_[step] = source;
                   anchor.enter(step); // Capture the voltage from this source

                   signals.start_mode_[step] = AnchorMode::Track;
                   signals.start_source_[step] = AnchorSource::Level;
                   auto constexpr current_level_voltage =
                       default_entry_voltage + 1.F;
                   signals.start_level_[step] = current_level_voltage;
                   auto const got = anchor.voltage();
                   auto constexpr want = current_level_voltage;
                   if (got != want) {
                     t.errorf("With source {} got anchor voltage {}, want {} ",
                              name_of(source), got, want);
                   }
                 }
               }));

    t.run("AnchorType::Start: "
          "voltage() with AnchorMode::Track and AnchorSource::InA: "
          "is current A voltage",
          test(AnchorType::Start, [](Tester &t, Signals &signals,
                                     Anchor &anchor) {
            auto constexpr step = 5;
            auto constexpr default_entry_voltage = 5.343F;

            for (auto const source : anchor_sources) {
              set_all_voltages(signals, default_entry_voltage);
              signals.start_source_[step] = source;
              anchor.enter(step); // Capture the voltage from this source

              signals.start_mode_[step] = AnchorMode::Track;
              signals.start_source_[step] = AnchorSource::InA;
              auto constexpr current_in_a_voltage = default_entry_voltage + 1.F;
              signals.in_a_ = current_in_a_voltage;
              auto const got = anchor.voltage();
              auto constexpr want = current_in_a_voltage;
              if (got != want) {
                t.errorf("With source {} got anchor voltage {}, want {} ",
                         name_of(source), got, want);
              }
            }
          }));

    t.run("AnchorType::Start: "
          "voltage() with AnchorMode::Track and AnchorSource::InB: "
          "is current B voltage",
          test(AnchorType::Start, [](Tester &t, Signals &signals,
                                     Anchor &anchor) {
            auto constexpr step = 5;
            auto constexpr default_entry_voltage = 5.343F;

            for (auto const source : anchor_sources) {
              set_all_voltages(signals, default_entry_voltage);
              signals.start_source_[step] = source;
              anchor.enter(step); // Capture the voltage from this source

              signals.start_mode_[step] = AnchorMode::Track;
              signals.start_source_[step] = AnchorSource::InB;
              auto constexpr current_in_b_voltage = default_entry_voltage + 1.F;
              signals.in_b_ = current_in_b_voltage;
              auto const got = anchor.voltage();
              auto constexpr want = current_in_b_voltage;
              if (got != want) {
                t.errorf("With source {} got anchor voltage {}, want {} ",
                         name_of(source), got, want);
              }
            }
          }));

    t.run("AnchorType::Start: "
          "voltage() with AnchorMode::Track and AnchorSource::InC: "
          "is current C voltage",
          test(AnchorType::Start, [](Tester &t, Signals &signals,
                                     Anchor &anchor) {
            auto constexpr step = 5;
            auto constexpr default_entry_voltage = 5.343F;

            for (auto const source : anchor_sources) {
              set_all_voltages(signals, default_entry_voltage);
              signals.start_source_[step] = source;
              anchor.enter(step); // Capture the voltage from this source

              signals.start_mode_[step] = AnchorMode::Track;
              signals.start_source_[step] = AnchorSource::InC;
              auto constexpr current_in_c_voltage = default_entry_voltage + 1.F;
              signals.in_c_ = current_in_c_voltage;
              auto const got = anchor.voltage();
              auto constexpr want = current_in_c_voltage;
              if (got != want) {
                t.errorf("With source {} got anchor voltage {}, want {} ",
                         name_of(source), got, want);
              }
            }
          }));

    t.run("AnchorType::Start: "
          "voltage() with AnchorMode::Track and AnchorSource::Out: "
          "is current OUT voltage",
          test(AnchorType::Start,
               [](Tester &t, Signals &signals, Anchor &anchor) {
                 auto constexpr step = 6;
                 auto constexpr default_entry_voltage = 7.343F;

                 for (auto const source : anchor_sources) {
                   set_all_voltages(signals, default_entry_voltage);
                   signals.start_source_[step] = source;
                   anchor.enter(step); // Capture the voltage from this source

                   signals.start_mode_[step] = AnchorMode::Track;
                   signals.start_source_[step] = AnchorSource::Out;
                   auto constexpr current_output_voltage =
                       default_entry_voltage + 1.F;
                   signals.output_ = current_output_voltage;
                   auto const got = anchor.voltage();
                   auto constexpr want = current_output_voltage;
                   if (got != want) {
                     t.errorf("With source {} got anchor voltage {}, want {} ",
                              name_of(source), got, want);
                   }
                 }
               }));

    t.run(
        "AnchorType::End: "
        "entered with AnchorSource::Level: "
        "voltage() with AnchorMode::Sample: "
        "is sampled level voltage",
        test(AnchorType::End, [](Tester &t, Signals &signals, Anchor &anchor) {
          auto constexpr step = 7;
          auto constexpr default_entry_voltage = 3.343F;

          set_all_voltages(signals, default_entry_voltage);
          auto constexpr level_entry_voltage = default_entry_voltage + 1.F;
          signals.end_source_[step] = AnchorSource::Level;
          signals.end_level_[step] = level_entry_voltage;
          anchor.enter(step);

          set_all_voltages(signals, level_entry_voltage + 1.F);

          signals.start_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            signals.end_source_[step] = source;
            auto const got = anchor.voltage();
            auto constexpr want = level_entry_voltage;
            if (got != want) {
              t.errorf("With source {} got anchor voltage {}, want {} ",
                       name_of(source), got, want);
            }
          }
        }));

    t.run(
        "AnchorType::End: "
        "entered with AnchorSource::InA: "
        "voltage() with AnchorMode::Sample: "
        "is sampled A voltage",
        test(AnchorType::End, [](Tester &t, Signals &signals, Anchor &anchor) {
          auto constexpr step = 6;
          auto constexpr default_entry_voltage = 4.343F;

          set_all_voltages(signals, default_entry_voltage);
          auto constexpr in_a_entry_voltage = default_entry_voltage + 1.F;
          signals.in_a_ = in_a_entry_voltage;
          signals.end_source_[step] = AnchorSource::InA;
          anchor.enter(step);

          set_all_voltages(signals, in_a_entry_voltage + 1.F);

          signals.start_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            signals.end_source_[step] = source;
            auto const got = anchor.voltage();
            auto constexpr want = in_a_entry_voltage;
            if (got != want) {
              t.errorf("With source {} got anchor voltage {}, want {} ",
                       name_of(source), got, want);
            }
          }
        }));

    t.run(
        "AnchorType::End: "
        "entered with AnchorSource::InB: "
        "voltage() with AnchorMode::Sample: "
        "is sampled A voltage",
        test(AnchorType::End, [](Tester &t, Signals &signals, Anchor &anchor) {
          auto constexpr step = 6;
          auto constexpr default_entry_voltage = 4.343F;

          set_all_voltages(signals, default_entry_voltage);
          auto constexpr in_b_entry_voltage = default_entry_voltage + 1.F;
          signals.in_b_ = in_b_entry_voltage;
          signals.end_source_[step] = AnchorSource::InB;
          anchor.enter(step);

          set_all_voltages(signals, in_b_entry_voltage + 1.F);

          signals.start_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            signals.end_source_[step] = source;
            auto const got = anchor.voltage();
            auto constexpr want = in_b_entry_voltage;
            if (got != want) {
              t.errorf("With source {} got anchor voltage {}, want {} ",
                       name_of(source), got, want);
            }
          }
        }));

    t.run(
        "AnchorType::End: "
        "entered with AnchorSource::InC: "
        "voltage() with AnchorMode::Sample: "
        "is sampled C voltage",
        test(AnchorType::End, [](Tester &t, Signals &signals, Anchor &anchor) {
          auto constexpr step = 6;
          auto constexpr default_entry_voltage = 4.343F;

          set_all_voltages(signals, default_entry_voltage);
          auto constexpr in_c_entry_voltage = default_entry_voltage + 1.F;
          signals.in_c_ = in_c_entry_voltage;
          signals.end_source_[step] = AnchorSource::InC;
          anchor.enter(step);

          set_all_voltages(signals, in_c_entry_voltage + 1.F);

          signals.start_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            signals.end_source_[step] = source;
            auto const got = anchor.voltage();
            auto constexpr want = in_c_entry_voltage;
            if (got != want) {
              t.errorf("With source {} got anchor voltage {}, want {} ",
                       name_of(source), got, want);
            }
          }
        }));

    t.run(
        "AnchorType::End: "
        "entered with AnchorSource::Out: "
        "voltage() with AnchorMode::Sample: "
        "is sampled OUT voltage",
        test(AnchorType::End, [](Tester &t, Signals &signals, Anchor &anchor) {
          auto constexpr step = 5;
          auto constexpr default_entry_voltage = 4.343F;

          set_all_voltages(signals, default_entry_voltage);
          auto constexpr output_entry_voltage = default_entry_voltage + 1.F;
          signals.output_ = output_entry_voltage;
          signals.end_source_[step] = AnchorSource::Out;
          anchor.enter(step);

          set_all_voltages(signals, output_entry_voltage + 1.F);

          signals.start_mode_[step] = AnchorMode::Sample;
          for (auto const source : anchor_sources) {
            signals.end_source_[step] = source;
            auto const got = anchor.voltage();
            auto constexpr want = output_entry_voltage;
            if (got != want) {
              t.errorf("With source {} got anchor voltage {}, want {} ",
                       name_of(source), got, want);
            }
          }
        }));

    t.run(
        "AnchorType::End: "
        "voltage() with AnchorMode::Track and AnchorSource::Level: "
        "is current level voltage",
        test(AnchorType::End, [](Tester &t, Signals &signals, Anchor &anchor) {
          auto constexpr step = 3;
          auto constexpr default_entry_voltage = 6.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(signals, default_entry_voltage);
            signals.end_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            signals.end_mode_[step] = AnchorMode::Track;
            signals.end_source_[step] = AnchorSource::Level;
            auto constexpr current_level_voltage = default_entry_voltage + 1.F;
            signals.end_level_[step] = current_level_voltage;
            auto const got = anchor.voltage();
            auto constexpr want = current_level_voltage;
            if (got != want) {
              t.errorf("With source {} got anchor voltage {}, want {} ",
                       name_of(source), got, want);
            }
          }
        }));

    t.run(
        "AnchorType::End: "
        "voltage() with AnchorMode::Track and AnchorSource::InA: "
        "is current A voltage",
        test(AnchorType::End, [](Tester &t, Signals &signals, Anchor &anchor) {
          auto constexpr step = 2;
          auto constexpr default_entry_voltage = 5.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(signals, default_entry_voltage);
            signals.end_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            signals.end_mode_[step] = AnchorMode::Track;
            signals.end_source_[step] = AnchorSource::InA;
            auto constexpr current_in_a_voltage = default_entry_voltage + 1.F;
            signals.in_a_ = current_in_a_voltage;
            auto const got = anchor.voltage();
            auto constexpr want = current_in_a_voltage;
            if (got != want) {
              t.errorf("With source {} got anchor voltage {}, want {} ",
                       name_of(source), got, want);
            }
          }
        }));

    t.run(
        "AnchorType::End: "
        "voltage() with AnchorMode::Track and AnchorSource::InB: "
        "is current B voltage",
        test(AnchorType::End, [](Tester &t, Signals &signals, Anchor &anchor) {
          auto constexpr step = 2;
          auto constexpr default_entry_voltage = 5.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(signals, default_entry_voltage);
            signals.end_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            signals.end_mode_[step] = AnchorMode::Track;
            signals.end_source_[step] = AnchorSource::InB;
            auto constexpr current_in_b_voltage = default_entry_voltage + 1.F;
            signals.in_b_ = current_in_b_voltage;
            auto const got = anchor.voltage();
            auto constexpr want = current_in_b_voltage;
            if (got != want) {
              t.errorf("With source {} got anchor voltage {}, want {} ",
                       name_of(source), got, want);
            }
          }
        }));

    t.run(
        "AnchorType::End: "
        "voltage() with AnchorMode::Track and AnchorSource::InC: "
        "is current C voltage",
        test(AnchorType::End, [](Tester &t, Signals &signals, Anchor &anchor) {
          auto constexpr step = 2;
          auto constexpr default_entry_voltage = 5.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(signals, default_entry_voltage);
            signals.end_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            signals.end_mode_[step] = AnchorMode::Track;
            signals.end_source_[step] = AnchorSource::InC;
            auto constexpr current_in_c_voltage = default_entry_voltage + 1.F;
            signals.in_c_ = current_in_c_voltage;
            auto const got = anchor.voltage();
            auto constexpr want = current_in_c_voltage;
            if (got != want) {
              t.errorf("With source {} got anchor voltage {}, want {} ",
                       name_of(source), got, want);
            }
          }
        }));

    t.run(
        "AnchorType::End: "
        "voltage() with AnchorMode::Track and AnchorSource::Out: "
        "is current OUT voltage",
        test(AnchorType::End, [](Tester &t, Signals &signals, Anchor &anchor) {
          auto constexpr step = 1;
          auto constexpr default_entry_voltage = 7.343F;

          for (auto const source : anchor_sources) {
            set_all_voltages(signals, default_entry_voltage);
            signals.end_source_[step] = source;
            anchor.enter(step); // Capture the voltage from this source

            signals.end_mode_[step] = AnchorMode::Track;
            signals.end_source_[step] = AnchorSource::Out;
            auto constexpr current_output_voltage = default_entry_voltage + 1.F;
            signals.output_ = current_output_voltage;
            auto const got = anchor.voltage();
            auto constexpr want = current_output_voltage;
            if (got != want) {
              t.errorf("With source {} got anchor voltage {}, want {} ",
                       name_of(source), got, want);
            }
          }
        }));
  }
};

static auto _ = AnchorSuite{};
} // namespace sequencizer
} // namespace test
