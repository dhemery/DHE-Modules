#pragma once

#include "rack.hpp"

namespace DHE {

    // In follower mode, a freezer yield's the supplier's value.
    // In frozen mode, a freezer yields a stored value.
    struct Freezer {
        explicit Freezer(const std::function<float()> &supplier);

        // Stores the supplier's current value and freezes this freezer.
        // Subsequent calls to value() will return the stored value.
        void freeze();

        // Unfreezes this freezer. Subsequent calls to value() will follow
        // supplier's value.
        void follow();

        // If this freezer is frozen, returns the value stored by the previous
        // freeze(). Otherwise returns the supplier's current value.
        float value();

    private:
        std::function<float()> freshValue;
        bool frozen;
        float frozenValue;
    };
}
