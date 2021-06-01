#include "./fixtures/sequence-controller-fixture.h"
#include <functional>
#include <helpers/latches.h>

namespace test {
namespace sequencizer {
using dhe::unit::Suite;
using dhe::unit::Tester;
using TestFunc = std::function<void(Tester &)>;

static inline void when_idle(Module &module, StepSelector & /*step_selector*/,
                             StepController & /*step_controller*/,
                             SequenceController &sequence_controller) {
  module.running_ = false;
  module.reset_ = false;
  sequence_controller.execute(0.F);
}

class SequenceControllerIdleSuite : Suite {
public:
  SequenceControllerIdleSuite()
      : Suite{"dhe::sequencizer::SequenceController: idle"} {}

  void run(Tester &t) override {
    t.run("with run high: with reset low: does nothing",
          test(when_idle, [](Tester &t, Module &module,
                             StepSelector &step_selector,
                             StepController &step_controller,
                             SequenceController &sequence_controller) {
            module.running_ = true;
            module.reset_ = false;

            auto constexpr original_output = -99342.2F;
            module.output_ = original_output;

            sequence_controller.execute(0.1F);

            if (step_selector.called_) {
              t.error("step selector was called");
            }
            if (step_controller.called_) {
              t.error("step controller was called");
            }
            if (module.output_ != original_output) {
              t.errorf("module output was changed (to {})", module.output_);
            }
          }));

    t.run("with run high: with gate low: does nothing",
          test(when_idle, [](Tester &t, Module &module,
                             StepSelector &step_selector,
                             StepController &step_controller,
                             SequenceController &sequence_controller) {
            module.running_ = true;
            module.gate_ = false;

            auto constexpr original_output = -992.223F;
            module.output_ = original_output;

            sequence_controller.execute(0.1F);

            if (step_selector.called_) {
              t.error("step selector was called");
            }
            if (step_controller.called_) {
              t.error("step controller was called");
            }
            if (module.output_ != original_output) {
              t.errorf("module output was changed (to {})", module.output_);
            }
          }));

    t.run("with run high: "
          "if gate rises: executes first step with gate edge cleared",
          test(when_idle,
               [](Tester &t, Module &module, StepSelector &step_selector,
                  StepController &step_controller,
                  SequenceController &sequence_controller) {
                 module.running_ = true;
                 module.gate_ = true;

                 auto constexpr first_enabled_step = 3;
                 step_selector.first_ = first_enabled_step;

                 step_controller.status_ = StepStatus::Generating;

                 auto constexpr sample_time = 0.39947F;
                 sequence_controller.execute(sample_time);

                 if (step_controller.entered_step_ != first_enabled_step) {
                   t.errorf("Entered step {}, want step {}",
                            step_controller.entered_step_, first_enabled_step);
                 }
                 if (step_controller.executed_latch_ != high_latch) {
                   t.errorf("Executed latch was {}, want {}",
                            step_controller.executed_latch_, high_latch);
                 }
                 if (step_controller.executed_sample_time_ != sample_time) {
                   t.errorf("Executed sample time was {}, want {}",
                            step_controller.executed_sample_time_, sample_time);
                 }
               }));

    t.run("with run high: "
          "if gate rises: does nothing if no first step",
          test(when_idle, [](Tester &t, Module &module,
                             StepSelector &step_selector,
                             StepController &step_controller,
                             SequenceController &sequence_controller) {
            module.running_ = true;
            module.gate_ = true;

            step_selector.first_ = -1;

            auto constexpr original_output = -2340.223F;
            module.output_ = original_output;

            sequence_controller.execute(0.F);

            if (step_controller.called_) {
              t.error("step controller was called");
            }
            if (module.output_ != original_output) {
              t.errorf("module output was changed (to {})", module.output_);
            }
          }));

    t.run("with run low: with reset low: does nothing",
          test(when_idle, [](Tester &t, Module &module,
                             StepSelector &step_selector,
                             StepController &step_controller,
                             SequenceController &sequence_controller) {
            module.running_ = false;
            module.reset_ = false;

            auto constexpr original_output = 349.319F;
            module.output_ = original_output;
            sequence_controller.execute(0.1F);

            if (step_selector.called_) {
              t.error("step selector was called");
            }
            if (step_controller.called_) {
              t.error("step controller was called");
            }
            if (module.output_ != original_output) {
              t.errorf("module output was changed (to {})", module.output_);
            }
          }));

    t.run("with run low: "
          "if reset rises: "
          "does nothing",
          test(when_idle, [](Tester &t, Module &module,
                             StepSelector &step_selector,
                             StepController &step_controller,
                             SequenceController &sequence_controller) {
            module.running_ = false;
            module.reset_ = true;

            auto constexpr original_output = 349.319F;
            module.output_ = original_output;
            sequence_controller.execute(0.1F);

            if (step_selector.called_) {
              t.error("step selector was called");
            }
            if (step_controller.called_) {
              t.error("step controller was called");
            }
            if (module.output_ != original_output) {
              t.errorf("module output was changed (to {})", module.output_);
            }
          }));

    t.run("with run low: with gate low: does nothing",
          test(when_idle, [](Tester &t, Module &module,
                             StepSelector &step_selector,
                             StepController &step_controller,
                             SequenceController &sequence_controller) {
            module.running_ = false;
            module.gate_ = false;

            auto constexpr original_output = 349.319F;
            module.output_ = original_output;
            sequence_controller.execute(0.1F);

            if (step_selector.called_) {
              t.error("step selector was called");
            }
            if (step_controller.called_) {
              t.error("step controller was called");
            }
            if (module.output_ != original_output) {
              t.errorf("module output was changed (to {})", module.output_);
            }
          }));

    t.run("with run low: if gate rises: does nothing",
          test(when_idle, [](Tester &t, Module &module,
                             StepSelector &step_selector,
                             StepController &step_controller,
                             SequenceController &sequence_controller) {
            module.gate_ = true;

            auto constexpr original_output = 349.319F;
            module.output_ = original_output;
            sequence_controller.execute(0.1F);

            if (step_selector.called_) {
              t.error("step selector was called");
            }
            if (step_controller.called_) {
              t.error("step controller was called");
            }
            if (module.output_ != original_output) {
              t.errorf("module output was changed (to {})", module.output_);
            }
          }));
  }
};

static auto _ = SequenceControllerIdleSuite{};
} // namespace sequencizer
} // namespace test
