#pragma once

#include <functional>
#include <utility>

namespace DHE {

    /**
     * A follower operates in one of two modes: following and paused.
     * While following, a follower yields the supplied value.
     * While paused, it yields the value it stored when it paused.
     *
     * A newly constructed follower is paused.
     */
    struct Follower {
        /**
         * Creates a follower supplied by the given function.
         * @param supplier the function whose values to follow
         */
        explicit Follower(std::function<float()> supplier) :
                suppliedValue(std::move(supplier)),
                storedValue(supplier()) {
        }

        /**
         * Stores the supplied value and pauses. Subsequent calls to
         * value() yield the stored value.
         */
        void pause() {
            storedValue = suppliedValue();
            following = false;
        }

        /**
         * Begins following the supplier. Subsequent calls to value()
         * yield the supplied value.
         */
        void follow() {
            following = true;
        }

        /**
         * Returns the supplied value if the follower is in follower mode.
        // Otherwise returns the value stored by the previous pause().
         * @return the supplied value if the follower is following,
         * otherwise the stored value.
         */
        float value() {
            return following ? suppliedValue() : storedValue;
        }

    private:
        bool following = false;
        std::function<float()> suppliedValue;
        float storedValue;
    };
}
