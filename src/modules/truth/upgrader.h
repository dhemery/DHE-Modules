#pragma once

#include "control-ids.h"
#include "outcomes.h"

namespace dhe {
namespace truth {
namespace upgrade {

static inline auto outcome_v1(bool q_true_outcome_v0, bool q_false_outcome_v0)
    -> float {
  if (q_true_outcome_v0 && q_false_outcome_v0) {
    // v0 outcome is true regardless of Q, so v1 outcome is True
    return static_cast<float>(Outcome::True);
  }
  if (!q_true_outcome_v0 && !q_false_outcome_v0) {
    // v0 outcome is false regardless of Q, so v1 outcome is False
    return static_cast<float>(Outcome::False);
  }
  // v0 outcomes differ depending on Q...
  if (q_true_outcome_v0) {
    // v0 outcome is true when Q is true, so v1 outcome is Q
    return static_cast<float>(Outcome::Q);
  }
  // v0 outcome is false when Q is true, so v1 outcome is QNot
  return static_cast<float>(Outcome::QNot);
}

template <int N, typename P> auto outcome_v0(P &params, int i) -> float {
  return params[ParamIds<N>::Outcome + i].getValue() > 0.5F;
}

template <int N, typename P> void upgrade_a(P &params) {
  static auto constexpr rows = 1 << N;
  // To improve the U/X, version 1 swaps version 0's positions of the T and F
  // outcomes in order to place T first in the toggle sequence: T F Q ¬Q.
  for (int i = 0; i < rows; i++) {
    auto const old_value = params[ParamIds<N>::Outcome + i].getValue();
    params[ParamIds<N>::Outcome + i].setValue(1.F - old_value);
  }
}

template <int N, typename P> void upgrade_q(P &params) {
  // Version 0's A/Q switch param is now used for the QOverride momentary
  // button. Make sure the button starts released.
  params[ParamIds<N>::QOverride].setValue(0.F);

  static auto constexpr q_false_rows = 1 << (N - 1);

  for (int q_false_row = 0; q_false_row < q_false_rows; q_false_row++) {
    auto const q_true_row = q_false_row + q_false_rows;
    auto const q_false_outcome_v0 = outcome_v0<N>(params, q_false_row);
    auto const q_true_outcome_v0 = outcome_v0<N>(params, q_true_row);
    auto const outcome_v1 =
        upgrade::outcome_v1(q_true_outcome_v0, q_false_outcome_v0);
    params[ParamIds<N>::Outcome + q_false_row].setValue(outcome_v1);
    params[ParamIds<N>::Outcome + q_true_row].setValue(outcome_v1);
  }
}

template <int N, typename P> void from_v0(P &params) {
  static auto constexpr aq_switch = ParamIds<N>::QOverride;
  auto const outcome_depends_on_q = value_of(params[aq_switch]) != 0.F;
  if (outcome_depends_on_q) {
    upgrade_q<N>(params);
  } else {
    upgrade_a<N>(params);
  }
}
} // namespace upgrade
} // namespace truth
} // namespace dhe
