#include "modules/sequencizer/anchor.h"

#include "dheunit/test.h"
#include "helpers/assertions.h"

#include <array>
#include <cstdlib>
#include <map>
#include <string>

namespace test {
namespace sequencizer {
using dhe::sequencizer::Anchor;
using dhe::sequencizer::AnchorMode;
using dhe::sequencizer::AnchorSource;
using dhe::sequencizer::AnchorType;
using dhe::unit::Suite;
using dhe::unit::Tester;

struct AnchorControls {
  AnchorMode mode;     // NOLINT
  AnchorSource source; // NOLINT
  float level;         // NOLINT

  void set(AnchorMode mode, AnchorSource source, float voltage) {
    this->mode = mode;
    this->source = source;
    if (source == AnchorSource::Level) {
      level = voltage;
    }
  }

  void clear() { level = -5555.F; }
};

struct Step {
  std::map<AnchorType, AnchorControls> anchor_controls; // NOLINT

  void set(AnchorType type, AnchorMode mode, AnchorSource source,
           float voltage) {
    anchor_controls[type].set(mode, source, voltage);
  }

  void clear() {
    for (auto &c : anchor_controls) {
      c.second.clear();
    }
  }
};

struct Signals {
  static auto constexpr step_count = 8;

  auto anchor_level(AnchorType type, int step) const -> float {
    return steps_[step].anchor_controls.at(type).level;
  }
  auto anchor_mode(AnchorType type, int step) const -> AnchorMode {
    return steps_[step].anchor_controls.at(type).mode;
  }
  auto anchor_source(AnchorType type, int step) const -> AnchorSource {
    return steps_[step].anchor_controls.at(type).source;
  }
  auto in_a() const -> float { return source_voltages_.at(AnchorSource::InA); }
  auto in_b() const -> float { return source_voltages_.at(AnchorSource::InB); }
  auto in_c() const -> float { return source_voltages_.at(AnchorSource::InC); }
  auto output() const -> float {
    return source_voltages_.at(AnchorSource::Out);
  }

  void set(AnchorType type, int step, AnchorMode mode, AnchorSource source,
           float voltage) {
    for (auto &s : steps_) {
      s.clear();
    }
    for (auto &source_voltage : source_voltages_) {
      source_voltage.second = -8888.F;
    }

    steps_[step].set(type, mode, source, voltage);
    source_voltages_[source] = voltage;

    source_voltages_[AnchorSource::Level] = -7777.F;
  }

private:
  std::map<AnchorSource, float> source_voltages_{};
  std::array<Step, step_count> steps_;
};

struct AnchorSampleTest {
  AnchorSource source_on_entry; // NOLINT
  float voltage_on_entry;       // NOLINT

  void run(Tester &t, AnchorType type) const {
    auto const name = std::string{"AnchorMode::Sample AnchorType::"} +
                      dhe::sequencizer::anchor_type::label(type) + " sampled " +
                      dhe::sequencizer::anchor_source::label(source_on_entry);
    t.run(name, [this, type](Tester &t) {
      static auto const step = std::rand() % Signals::step_count;
      auto signals = Signals{};
      auto anchor = Anchor<Signals>{signals, type};

      signals.set(type, step, AnchorMode::Sample, source_on_entry,
                  voltage_on_entry);

      anchor.enter(step);

      // The anchor must report the sampled voltage, regardless of the current
      // source.
      for (auto const &current_source :
           dhe::sequencizer::anchor_source::values) {
        signals.set(type, step, AnchorMode::Sample, current_source,
                    voltage_on_entry + 222.F);

        auto const got = anchor.voltage();
        if (got != voltage_on_entry) {
          assert_that(
              t,
              std::string{"With current source "} +
                  dhe::sequencizer::anchor_source::label(current_source),
              got, is_equal_to(voltage_on_entry));
        }
      }
    });
  }
};

auto anchor_sample_tests = std::vector<AnchorSampleTest>{
    {AnchorSource::Level, 3.983F}, {AnchorSource::InA, 2.983F},
    {AnchorSource::InB, 1.983F},   {AnchorSource::InC, -1.983F},
    {AnchorSource::Out, -3.983F},
};

class AnchorSuite : public Suite {
public:
  AnchorSuite() : Suite("dhe::sequencizer::Anchor::voltage()") {}
  void run(Tester &t) override {
    for (auto const &test : anchor_sample_tests) {
      test.run(t, AnchorType::Start);
      test.run(t, AnchorType::End);
    }
  }
};
/**
    t.run("AnchorType::Start: "
          "voltage() with AnchorMode::Track and AnchorSource::Level: "
          "is current level voltage",
          test(AnchorType::Start, [](Tester &t, Signals &signals,
                                     Anchor &anchor) {
            auto constexpr step = 4;
            auto constexpr default_entry_voltage = 6.343F;

            for (auto const source : dhe::sequencizer::anchor_source::values) {
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
                         source, got, want);
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

            for (auto const source : dhe::sequencizer::anchor_source::values) {
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
                         source, got, want);
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

            for (auto const source : dhe::sequencizer::anchor_source::values) {
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
                         source, got, want);
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

            for (auto const source : dhe::sequencizer::anchor_source::values) {
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
                         source, got, want);
              }
            }
          }));

    t.run("AnchorType::Start: "
          "voltage() with AnchorMode::Track and AnchorSource::Out: "
          "is current OUT voltage",
          test(AnchorType::Start, [](Tester &t, Signals &signals,
                                     Anchor &anchor) {
            auto constexpr step = 6;
            auto constexpr default_entry_voltage = 7.343F;

            for (auto const source : dhe::sequencizer::anchor_source::values) {
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
                         source, got, want);
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

          for (auto const source : dhe::sequencizer::anchor_source::values) {
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
              t.errorf("With source {} got anchor voltage {}, want {} ", source,
                       got, want);
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

          for (auto const source : dhe::sequencizer::anchor_source::values) {
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
              t.errorf("With source {} got anchor voltage {}, want {} ", source,
                       got, want);
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

          for (auto const source : dhe::sequencizer::anchor_source::values) {
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
              t.errorf("With source {} got anchor voltage {}, want {} ", source,
                       got, want);
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

          for (auto const source : dhe::sequencizer::anchor_source::values) {
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
              t.errorf("With source {} got anchor voltage {}, want {} ", source,
                       got, want);
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

          for (auto const source : dhe::sequencizer::anchor_source::values) {
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
              t.errorf("With source {} got anchor voltage {}, want {} ", source,
                       got, want);
            }
          }
        }));
  }
};
*/

static auto _ = AnchorSuite{};
} // namespace sequencizer
} // namespace test
