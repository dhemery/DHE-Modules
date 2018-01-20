# pragma once

#include <functional>
#include "rack.hpp"

namespace DHE {
    struct SwitchedOutput {
        SwitchedOutput(rack::Output &output, const std::function<float()> &trueValue,
                       const std::function<float()> &falseValue);

        void send(bool state);

        static std::unique_ptr<SwitchedOutput> gate(rack::Output &output);

        static std::unique_ptr<SwitchedOutput> pulse(rack::Output &output);

        static std::unique_ptr<SwitchedOutput>
        choosing(rack::Output &output,
                 const std::function<float()> &trueValue,
                 const std::function<float()> &falseValue);
    private:
        rack::Output *output;
        std::function<float()> falseValue;

        std::function<float()> trueValue;
    };
};