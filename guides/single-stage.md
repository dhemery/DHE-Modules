You can create a simple envelopes
with one Stage module and (maybe) one Upstage module.

- [Attack and Hold](#attack-and-hold)
- [Decay and Hold](#decay-and-hold)
- [Rise/Fall and Hold](#rise-fall-and-hold)

**NOTES**

When a Stage module completes its envelope stage,
  it *holds* at the voltage of the *LEVEL* knob
  until it receives a trigger or its *DEFER* gate rises.

These single-stage envelopes are *retriggerable.*
  If a trigger arrives
  while the envelope stage is in progress,
  Stage begins a new envelope stage
  *starting at the current voltage on the *IN* port*.

## Attack and Hold

- Connect a trigger source to Stage's *TRIG* port.
- Leave the *IN* port disconnected.

![Attack and Hold](attack-and-hold.png)

When a trigger arrives,
Stage starts an envelope stage
that progresses from 0 volts to the *LEVEL* voltage,
and holds until the next trigger.

## Decay and Hold

- Connect the trigger source
  to both the *TRIG* port and the *IN* port:

![Decay and Hold](decay-and-hold.png)

Think of this as an Attack/Decay/Hold envelope
with instantaneous attack.

This is useful for plucks.

**NOTE:**
With the trigger source connected to the *IN* port,
the envelope stage starts
at whatever voltage the trigger source
offers *at the instant it rises to or above 1 volt.*
So you can vary the envelope stage's starting voltage
by varying the incoming trigger voltage.

## Rise/Fall and Hold

Combine an Upstage module with a Stage module
to start and end the envelope at whatever levels you like.

Here is a rising single-stage envelope:

![Rising Envelope with Upstage](upstage-rise.png)

Here is a falling single-stage envelope:

![Falling Envelope with Upstage](upstage-fall.png)
