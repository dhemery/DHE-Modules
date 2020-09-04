#pragma once

namespace dhe {
namespace curve_sequencer {

enum class AnchorSource { Level, In, Out };

static auto constexpr anchor_source_count =
    static_cast<int>(AnchorSource::Out) + 1;

} // namespace curve_sequencer
} // namespace dhe
