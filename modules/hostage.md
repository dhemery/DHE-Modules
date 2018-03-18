---
title: The Hostage Module
---
<img class="panel" src="panel.svg" alt="The Hostage Panel" />

Generates an envelope hold stage (aka sustain stage).

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

## Hold Modes

- In **GATE** mode,
    _Hostage_ treats the _HOLD_ signal as a sustain gate.
    When the gate goes high,
    _Hostage_ samples the _IN_ signal,
    then generates a sustain stage
    at the sampled voltage
    while gate remains high.

- In **TIMER** mode,
    _Hostage_ treats the _HOLD_ signal as a trigger.
    When triggered,
    _Hostage_ samples the _IN_ signal,
    then generates a timed envelope stage
    at the sampled voltage.

## Controls

- **GATE / TIMER:**
    Selects _Hostage's_ operating mode.

- **DURATION:**
    The duration of the hold stage
    (when _Hostage_ is in _TIMER_ mode).

    The duration switch **(1 / 10 / 100)**
    sets the maximum value (seconds) of the _DURATION_ knob.
    The minimum value is 1/1000 of the maximum value
    (1ms, 10ms, or 100ms).
    At the center position,
    the duration is 1/10 of the maximum
    100ms, 1s, or 10s).

    In _GATE_ mode,
    _Hostage_ ignores the _DURATION_ controls.

## Inputs and Outputs

- **DEFER:**
    When the _DEFER_ gate signal is high,
    _Hostage_ **_defers_** to its _IN_ signal,
    forwarding the _IN_ signal
    directly to the _OUT_ output.

    While _Hostage_ is deferring
    it ignores incoming _HOLD_ signals.

    When it begins deferring,
     _Hostage_ abandons any hold stage
    it may have been generating.

- **HOLD:**
    Triggers or sustains a hold stage,
    depending on the current operating mode.

    While _Hostage_ is deferring,
    it ignores incoming _HOLD_ gates and triggers.

    In _HOLD_ mode,
    if the _HOLD_ gate is high
    when _Hostage_ stops deferring,
    it begins a new sustain stage.
    If the _HOLD_ gate is low
    when _Hostage_ stops deferring,
    it emits an _EOC_ pulse.

- **IN:**
    When _Hostage_ generates a hold stage
    (whether gated or timed),
    it emits the voltage
    sampled from the _IN_ signal
    at the start of the stage.

    While _Hostage_ is _deferring_,
    it forwards the _IN_ signal directly to its _OUT_ output.

- **ACTIVE:**
    A 10V gate signal indicating that _Hostage_
    is either actively generating a hold stage
    or deferring to its _IN_ signal.

- **EOC:**
    When _Hostage_ completes a hold stage
    (whether gated or timed),
    it emits a 1 millisecond 10V pulse
    at its _EOC_ output.

- **OUT:**
    The generated hold stage signal
    or (when deferring) the _IN_ signal.

- **DURATION CV:**
    Control voltage signal that modulates the _DURATION_ knob.
    A 0V _CV_ signal yields the value of the knob.
    The modulation is calibrated
    so that with the knob in the center position,
    a -5V _CV_ signal yields the knob's minimum value,
    and a 5V _CV_ signal yields its maximum value.
