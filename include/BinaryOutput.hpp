# pragma once

#include <functional>
#include "rack.hpp"

namespace DHE {
    struct BinaryOutput {
        BinaryOutput(rack::Output &output, float valueWhenTrue, float valueWhenFalse);
        void send(bool state);

        static std::unique_ptr<BinaryOutput> gate(rack::Output &output);

        static std::unique_ptr<BinaryOutput> pulse(rack::Output &output);

    private:
        rack::Output &output;
        const float valueWhenFalse;
        const float valueWhenTrue;
    };
};