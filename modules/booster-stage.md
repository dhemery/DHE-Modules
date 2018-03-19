---
title: The Booster Stage Module
---
<img class="panel" src="panel.svg" alt="The Booster Stage Panel" />

Generates a single envelope stage.

_Booster Stage_ features all of
_[Stage's]({{ '/modules/stage/' | relative_url }})_
functionality,
enhanced by
CV inputs,
sigmoid curves,
buttons to inject triggers and other control signals,
and more.

Combine
[Stage]({{ '/modules/stage/' | relative_url }}),
[Booster Stage]({{ '/modules/booster-stage/' | relative_url }}),
[Hostage]({{ '/modules/hostage/' | relative_url }}),
and [Upstage]({{ '/modules/upstage/' | relative_url }})
modules
to generate complex envelopes
with any number of stages.

See also:

- [Generating Single-Stage Envelopes]({{ '/guides/single-stage/' | relatuve_url }})
- [Generating Multi-Stage Envelopes]({{ '/guides/multi-stage/' | relative_url }})
- [How Stages Work]({{ '/technical/stages/' | relative_url }})

## Controls

- **LEVEL:**
    The voltage level at which each generated envelope stage ends.

    The polarity switch **(UNI / BI)**
    selects the range of the _LEVEL_ knob:
    Either unipolar control voltage (0 to 10V)
    or bipolar control voltage (-5 to 5V).

- **CURVE:**
    The curvature of the envelope stage.
    The center position (no curvature)
    produces a linear stage.
    Positive curvature produces an S- or J-shaped curve.
    Negative curvature produces an inverted S- or J-shaped curve.
    At extreme positions the curvature is severe.

    The shape switch **(S / J)**
    selects the shape of the envelope stage.
    _J_ selects a J-shaped curve
    reminiscent of a "logarithmic" curve.
    _S_ selects a sigmoid (S-shaped) curve.

- **DURATION:**
    The duration of the envelope stage.

    The duration switch **(1 / 10 / 100)**
    sets the maximum value (seconds) of the _DURATION_ knob.
    The minimum value is 1/1000 of the maximum value
    (1ms, 10ms, or 100ms).
    At the center position,
    the duration is 1/10 of the maximum
    100ms, 1s, or 10s).

- **Buttons:**
    The _DEFER_ and _TRIG_ inputs
    and the _ACTIVE_ and _EOC_ outputs
    each have an associated "manual override" button.
    While pressed,
    each button injects 10V into the associated signal
    (unless the signal exceeds 10V).

## Inputs and Outputs

- **DEFER:**
    When the _DEFER_ gate signal is high,
    _Booster Stage_ **_defers_** to its _IN_ signal,
    forwarding the _IN_ signal
    directly to the _OUT_ output.

    When it begins deferring,
     _Booster Stage_ abandons any envelope stage
    it may have been generating.

- **TRIG:**
    When triggered,
    _Booster Stage_ generates an envelope stage,
    starting at the voltage sampled from the _IN_ signal.

    While _Booster Stage_ is _deferring,_
    it ignores incoming triggers.

- **IN:**
    When _Booster Stage_ generates an envelope stage,
    the stage starts
    at the voltage sampled from the _IN_ signal.

    While _Booster Stage_ is deferring,
    it forwards the _IN_ signal directly to its _OUT_ output.

- **ACTIVE:**
    A 10V gate signal indicating that _Booster Stage_
    is either actively generating an envelope stage
    or deferring to its _IN_ signal.

- **EOC:**
    When _Booster Stage_ completes an envelope stage,
    it emits a 1 millisecond 10V pulse
    at its _EOC_ output.

- **OUT:**
    The generated envelope stage signal
    or (when deferring) the _IN_ signal.

- **CV:**
    Control voltage signal that modulates the associated knob.
    The modulation is calibrated so that:
    - A 0V _CV_ signal yields the value of the knob.
    - A 5V _CV_ signal yields the value
        as if the knob were rotated 50%
        clockwise
        of its actual position.
    - A -5V _CV_ signal yields the value
        as if the knob were rotated 50%
        counterclockwise
        of its actual position.
