---
title: The Stage Module
---

<img class="panel" src="panel.svg" alt="The Stage Panel" />

Generates a single envelope stage.

Combine
[_Stage_]({{ '/modules/stage/' | relative_url }}),
[_Booster Stage_]({{ '/modules/booster-stage/' | relative_url }}),
[_Hostage_]({{ '/modules/hostage/' | relative_url }}),
and [_Upstage_]({{ '/modules/upstage/' | relative_url }})
modules
to generate complex envelopes
with any number of stages.

See also:

- [Generating Single-Stage Envelopes]({{ '/guides/generating-single-stage-envelopes/' | relative_url }})
- [Generating Multi-Stage Envelopes]({{ '/guides/generating-multi-stage-envelopes/' | relative_url }})
- [How Stages Work Together]({{ 'technical/how-stages-work-together' | relative_url }})

## Controls

- **LEVEL:**
    The voltage level at which each generated envelope stage ends.
    The range is 0–10V.

- **CURVE:**
    The curvature of the envelope stage.
    The center position (no curvature)
    produces a linear stage.
    Positive curvature produces a J-shaped curve
    reminiscent of a "logarithmic" curve.
    Negative curvature produces an inverted J shape.
    At extreme positions the curvature is severe.

- **DURATION:**
    The duration of the envelope stage.
    The range is 10ms–10s,
    with the center position
    giving a duration of 1s.

## Ports

- **DEFER:**
    When the _DEFER_ gate signal is high,
    _Stage_ **_defers_** to its _IN_ signal,
    forwarding the _IN_ signal
    directly to the _OUT_ output.

    When it begins deferring,
    _Stage_ abandons any envelope stage
    it may have been generating.

- **TRIG:**
    When triggered,
    _Stage_ generates an envelope stage,
    starting at the voltage sampled from the _IN_ signal.

    While _Stage_ is deferring,
    it ignores incoming triggers.

- **IN:**
    When _Stage_ generates an envelope stage,
    the stage starts
    at the voltage sampled from the _IN_ signal.

    While _Stage_ is _deferring_,
    it forwards the _IN_ signal directly to its _OUT_ output.

- **ACTIVE:**
    A 10V gate signal indicating that _Stage_
    is either actively generating an envelope stage
    or deferring to its _IN_ signal.

- **EOC:**
    When _Stage_ completes an envelope stage,
    it emits a 1 millisecond 10V pulse
    at its _EOC_ output.

- **OUT:**
    The generated envelope stage signal
    or (when deferring) the _IN_ signal.

## Notes

- While an envelope stage is in progress,
    all of the knobs are "live."
    If you adjust a knob,
    _Stage_ applies the new value
    to the remainder of the stage.

- After _Stage_ completes an envelope stage,
    the _LEVEL_ knob remains live
    until the _DEFER_ gate goes high.
    If you adjust the _LEVEL_ knob,
    the _OUT_ signal follows.

- Before _Stage_ becomes active for the first time
    (by either a trigger or a _DEFER_ gate),
    it outputs 0V.
