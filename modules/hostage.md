---
title: The Hostage Module
---
<img class="panel" src="panel.svg" alt="The Hostage Panel" />

Generates a hold or sustain envelope stage.
A hold stage lasts for the duration specified by the controls.
A sustain stage lasts as long as the _GATE_ is up.

Combine
[Stage]({{ '/modules/stage/' | relative_url }}),
[Booster Stage]({{ '/modules/booster-stage/' | relative_url }}),
[Hostage]({{ '/modules/hostage/' | relative_url }}),
and [Upstage]({{ '/modules/upstage/' | relative_url }})
modules
to generate complex envelopes
with any number of stages.

See also:

- [Generating Single-Stage Envelopes]({{ '/guides/generating-single-stage-envelopes/' | relative_url }})
- [Generating Multi-Stage Envelopes]({{ '/guides/generating-multi-stage-envelopes/' | relative_url }})
- [How Stages Work Together]({{ 'technical/how-stages-work-together' | relative_url }})

## Modes

- In **SUSTAIN** mode,
    the _GATE_ signal acts as a sustain gate.
    When the gate rises,
    _Hostage_ samples the _IN_ signal,
    then generates a sustain stage
    at the sampled voltage
    until the gate falls.

- In **HOLD** mode,
    the _GATE_ signal acts as a trigger.
    When triggered,
    _Hostage_ samples the _IN_ signal,
    then generates a hold stage
    at the sampled voltage
    for the specified duration.

## Controls

- **SUSTAIN / HOLD:**
    Selects _Hostage's_ operating mode.

- **DURATION:**
    The duration of the hold stage
    (when _Hostage_ is in _HOLD_ mode).

    The duration switch **(1 / 10 / 100)**
    sets the range of the _DURATION_ knob.
    The switch labels
    indicate the maximum duration (seconds)
    for each range.
    For each range:
    - The minimum duration
        is 1/1000 of the maximum:
        1ms, 10ms, or 100ms.
    - Centering the knob gives a duration of
        1/10 of the maximum:
        100ms, 1s, or 10s.

    The _DURATION_ knob is modulated by the **CV** input.
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

    In _GATE_ mode,
    _Hostage_ ignores the _DURATION_ controls.

## Ports

- **DEFER:**
    When the _DEFER_ gate is up,
    _Hostage_ **_defers_** to its _IN_ signal,
    forwarding the _IN_ signal
    directly to the _OUT_ output.

    While _Hostage_ is deferring
    it ignores incoming _GATE_ signals.

    When it begins deferring,
     _Hostage_ abandons any envelope stage
    it may have been generating.

- **GATE:**
    Triggers a hold stage
    or sustains a sustain stage,
    depending on the current operating mode.

    While _Hostage_ is deferring,
    it ignores the incoming _GATE_ signal.

    In _SUSTAIN_ mode,
    if the _GATE_ is up
    when _Hostage_ stops deferring,
    it begins a new sustain stage.
    If the _GATE_ is down
    when _Hostage_ stops deferring,
    it emits an _EOC_ pulse.

- **IN:**
    When _Hostage_ generates a hold or sustain stage
    it emits the voltage
    that it sampled from the _IN_ signal
    at the start of the stage.

    While _Hostage_ is _deferring_,
    it forwards the _IN_ signal directly to its _OUT_ output.

- **ACTIVE:**
    A 10V gate signal indicating that _Hostage_
    is either actively generating a stage
    or deferring to its _IN_ signal.

- **EOC:**
    When _Hostage_ completes a stage
    it emits a 1 millisecond 10V pulse
    at its _EOC_ output.

- **OUT:**
    The generated stage signal
    or (when deferring) the _IN_ signal.

## Notes

- While a stage is in progress,
    the _DURATION_ knob, switch, and _CV_ input
    are "live."
    If the duration value changes,
    _Hostage_ applies the new value
    to the remainder of the stage.

- Before _Hostage_ becomes active for the first time
    (whether by a trigger, a gate, or a _DEFER_ signal),
    it outputs 0V.

- Changing the mode abandons any stage in progress.
