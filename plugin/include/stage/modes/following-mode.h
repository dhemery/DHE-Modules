#pragma once

#include "util/mode.h"
#include "components/trigger.h"

namespace DHE {

template <typename M> class FollowingMode : public Mode {
public:
  explicit FollowingMode(M *module)
      : module{module} {}

  void enter() override { module->set_active(false); }

  void step() override { module->do_follow(); }

private:
  M *module;
};
} // namespace DHE
