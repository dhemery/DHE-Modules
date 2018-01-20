#pragma once

#include <functional>

namespace DHE {

    // In follower mode, a freezer yields the supplied value. In frozen
    // mode, a freezer yields the value supplied at the time of freezing.
    struct Freezer {
        explicit Freezer(const std::function<float()> &supplier);

        // Stores the supplied value and enters frozen mode. Subsequent
        // calls to value() will return the stored value.
        void freeze();

        // Enters follower mode. Subsequent calls to value() will yield
        // the supplied value.
        void follow();

        // Returns the supplied value if the freezer is in follower mode.
        // Otherwise returns the value stored by the previous freeze().
        float value();

    private:
        std::function<float()> freshValue;
        bool frozen;
        float frozenValue;
    };
}
