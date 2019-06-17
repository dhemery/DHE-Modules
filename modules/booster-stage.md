---
title: The Booster Stage Module
---
<img class="faceplate" src="booster-stage.svg" alt="The Booster Stage Faceplate" />

Generates a single envelope stage.

_Booster Stage_ features all of
_[Stage's]({{ '/modules/stage/' | relative_url }})_
functionality,
enhanced by
CV inputs,
S curves,
buttons to inject triggers and other control signals,
and more.

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
- [J and S Curves]({{ '/technical/curves/' | relative_url }})


## Controls

- **LEVEL:**
    The voltage level at which each generated envelope stage ends.

    The polarity switch **(UNI / BI)**
    selects the range of the _LEVEL_ knob:
    Either unipolar control voltage (0 to 10V)
    or bipolar control voltage (-5 to 5V).

- **CURVE:**
    The
    [curvature]({{ '/technical/curves/' | relative_url }})
    of the envelope stage.
    The shape switch **(S / J)**
    selects the [shape]({{ '/technical/curves/' | relative_url }})
    of the envelope stage.

- **DURATION:**
    The duration of the envelope stage.

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

- **Buttons:**
    The _DEFER_ and _TRIG_ inputs
    and the _ACTIVE_ and _EOC_ outputs
    each have an associated "manual override" button.
    While pressed,
    each button injects 10V into the associated signal
    (unless the signal exceeds 10V).

## Ports

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
    A [control voltage]({{ '/technical/modulation/' | relative_url }})
    signal to modulate the associated parameter.


## Notes

- While an envelope stage is in progress,
  all of the knobs are "live."
  If you adjust a knob,
  _Booster Stage_ applies the new value
  to the remainder of the stage.

- Before _Booster Stage_ first becomes active
  (by either a trigger or a _DEFER_ gate),
  it "tracks" its input
  by sending its _IN_ voltage to its _OUT_ port.
  Stage also enters this mode
  when its _DEFER_ gate falls (unless the _TRIG_ port is high). 

- After _Booster Stage_ completes an envelope stage,
  it "tracks" the _LEVEL_ knob
  by sending the _LEVEL_ voltage to the _OUT_ port.

## State Model

![Stage State Model](/modules/stage/stage-state-model.png)

### TrackingInput Mode
_Booster Stage_ starts in _TrackingInput_ mode.
While _Booster Stage_ is _TrackingInput_:
- it is inactive
- it "tracks" its input by sending its _IN_ voltage to its _OUT_ port
- if _DEFER_ rises, _Booster Stage_ enters _Deferring_ mode 
- if _TRIG_ rises, _Booster Stage_ enters _Generating_ mode

### Deferring Mode

Whatever mode _Booster Stage_ is in,
whenever its _DEFER_ gate rises,
it enters _Deferring_ mode.
While _Booster Stage_ is _Deferring:

- it is active
- it sends its _IN_ voltage to its _OUT_ port
- it ignores its _TRIG_ port

When the _DEFER_ gate falls,
_Booster Stage_ resumes _TrackingInput_.

### Generating Mode

If the _TRIG_ port rises
while _Booster Stage_ is not _Deferring_,
it begins _Generating_ a stage.
While _Booster Stage_ is _Generating_:

- it is active
- it advances its phase and generates the appropriate voltage at its _OUT_ port
- if the stage completes,
  _Booster Stage_ initiates a short pulse on its _EOC_ port
  and enters _TrackingLevel_ mode
- if _DEFER_ rises, _Booster Stage_ enters _Deferring_ mode 
- if _TRIG_ rises, _Booster Stage_ abandons the stage it is generating and begins a new one


### TrackingLevel Mode

While _Booster Stage_ is _TrackingLevel_:

- it is inactive
- it sends the voltage of its _LEVEL_ knob to its _OUT_ port
- if _DEFER_ rises, _Booster Stage_ enters _Deferring_ mode 
- if _TRIG_ rises, _Booster Stage_ enters _Generating_ mode
