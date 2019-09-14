
#include <gtest/gtest.h>

namespace dhe {

template <typename M> class CurveSequencerEngine {
public:
  explicit CurveSequencerEngine(M &module, int stageCount) {}
};

class FakeCurveSequencer {
public:
  explicit FakeCurveSequencer(int stageCount) {}
};

struct CurveSequencerEngineTest : public ::testing::Test {
  FakeCurveSequencer sequencer{4};
  CurveSequencerEngine<FakeCurveSequencer> engine{sequencer, 4};
  void SetUp() override {}
};

TEST_F(CurveSequencerEngineTest, foo) {}

} // namespace dhe
