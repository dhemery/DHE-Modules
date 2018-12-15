#pragma once

#include "display/controls.h"
#include "range.h"
#include "sigmoid.h"

namespace DHE {

namespace Duration {
static constexpr auto short_range = Range{0.001f, 1.f};
static constexpr auto medium_range = Range{0.01f, 10.f};
static constexpr auto long_range = Range{0.1f, 100.f};

template <typename P, typename M> class RangeSelector : public Toggle<P, 3> {
public:
  void onChange(rack::EventChange &e) override {
    Toggle<P, 3>::onChange(e);
    dynamic_cast<M *>(this->module)->set_duration_range(this->selection());
  }

private:
  auto selection() const -> const Range & { return ranges[this->position()]; }

  const std::vector<const Range> ranges{short_range, medium_range, long_range};
};
} // namespace Duration

inline auto duration(float rotation, const Range &range) -> float {
  auto tapered = Sigmoid::j_taper(rotation, 0.8f);
  return range.scale(tapered);
} // namespace Duration

inline auto duration(float rotation) -> float {
  return duration(rotation, Duration::medium_range);
}
} // namespace DHE
