#include "dheunit/test.h"
#include "helpers/assertions.h"
#include "stage-engine-fixture.h"

namespace test {
namespace stage {

using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_true;

// Nothing to do, because the state is already freshly constructed
static inline void freshly_constructed(Signals & /**/, SimpleMode & /**/,
                                       SimpleMode & /**/, TimedMode & /**/,
                                       SimpleMode & /**/, StageEngine & /**/) {}

class StageEngineSuite : public Suite {
public:
  StageEngineSuite() : Suite{"dhe::envelope::stage::Engine"} {}
  void run(Tester &t) override {
    t.run("starts in input mode",
          test(freshly_constructed,
               [](Tester &t, Signals & /**/, SimpleMode & /**/,
                  SimpleMode &input_mode, TimedMode & /**/, SimpleMode & /**/,
                  StageEngine &engine) {
                 engine.process(0.F);
                 assert_that(t, input_mode.executed_, is_true);
               }));
  }
};

static auto _ = StageEngineSuite{};
} // namespace stage
} // namespace test
