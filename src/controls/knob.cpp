#include <engine.hpp>

namespace DHE {

auto default_cv() -> const rack::Input * {
  static auto cv_input = rack::Input{};
  cv_input.value = 0.f;
  return &cv_input;
}

auto default_av() -> const rack::Param * {
  static auto av_param = rack::Param{};
  av_param.value = 1.f;
  return &av_param;
}
} // namespace DHE
