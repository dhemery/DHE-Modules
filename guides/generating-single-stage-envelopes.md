---
title: Generating Single-Stage Envelopes
---

You can create a simple envelopes
with one
[_Stage_]({{ '/modules/stage/' | relative_url }})
module.
For additional control and versatility,
add an
[_Upstage_]({{ '/modules/upstage' | relative_url }})
or use a
[_Booster Stage_]({{ '/modules/booster-stage' | relative_url }})
instead of a _Stage_.

- [Attack and Hold](#attack-and-hold)
- [Decay and Hold](#decay-and-hold)
- [Setting the Starting Voltage](#setting-the-starting-voltage)
- [Preventing Retriggers](#preventing-retriggers)

## Attack and Hold

- Connect a trigger source to Stage's *TRIG* input.
- Leave the *IN* input disconnected.
- Set the _LEVEL_ knob to the desired _hold_ voltage.

![Attack and Hold Envelope](attack-and-hold-envelope.png)

When a trigger arrives,
_Stage_ generates an envelope stage
that progresses from 0 volts to the _LEVEL_ voltage.
It then continues to emit the _LEVEL_ voltage
until it receives the next trigger.

## Decay and Hold

- Connect a trigger source
  to both the *TRIG* input and the *IN* input:
- Set the _LEVEL_ knob to the desired _hold_ voltage.

This is useful for plucks:

![Decay and Hold Envelope](decay-and-hold-envelope.png)

Think of this as an Attack/Decay/Hold envelope
with instantaneous attack.


**Note:**
With the trigger source connected to the *IN* input,
the envelope stage starts
at whatever voltage the trigger source
offers *at the instant it rises to at least 1V.*
So you can vary the envelope stage's starting voltage
by varying the trigger voltage.

## Setting the Starting Voltage

_Stage's_ _LEVEL_ knob sets the ending voltage of the envelope stage.
To set the starting voltage,
add an _Upstage_
or other voltage source.

Here is a rising single-stage envelope:

![Rising Envelope with Upstage](upstage-rise.png)

Here is a falling single-stage envelope:

![Falling Envelope with Upstage](upstage-fall.png)

## Preventing Retriggers

Each single-stage envelope shown above is *retriggerable:*
If a trigger arrives
while an envelope stage is in progress,
_Stage_ interrupts the in-progress envelope stage
and begins a new one
at the voltage of the *IN* signal
at that instant.
