#ifndef DHE_MODULES_TEST_FAKE_STAGE_MODEL_H
#define DHE_MODULES_TEST_FAKE_STAGE_MODEL_H

namespace DHE {

struct FakeStageModel {
  float duration() { return 0.f; }
  float level() { return 0.f; }
  float shape() { return 0.f; }

  float defer_in() { return 0.f; }
  float stage_in() { return 0.f; }
  float trigger_in() { return 0.f; }

  float active_out() { return 0.f; }
  float eoc_out() { return 0.f; }
  float stage_out() { return 0.f; }

  float sample_time() { return 0.f; }
};
}

#endif //DHE_MODULES_FAKE_STAGE_MODEL_H
