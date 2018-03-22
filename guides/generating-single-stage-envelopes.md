---
title: Generating Single-Stage Envelopes
---

You can create a simple envelopes
with a single
[_Stage_]({{ '/modules/stage/' | relative_url }})
module:

- [Attack-and-Hold Envelopes](#ah).
- [Decay-and-Hold Envelopes](#dh).


For additional control and versatility,
use a
[_Booster Stage_]({{ '/modules/booster-stage' | relative_url }})
instead of a _Stage_.

**See also:**
[Techniques and Variations]({{ '/guides/techniques-and-variations/' | relative_url }}),
which describes
how to wire multi-stage envelopes,
how to add hold and sustain stages,
and how to create
retriggerable envelopes,
uninterruptible envelopes,
looping envelopes,
and other interesting variations.

## <span id="ah">Attack-and-Hold</span> Envelopes

<img src="ah.png" height="360" alt="Attack and Hold Envelope" />

- Connect a trigger source to the *TRIG* input.
- Leave the *IN* input disconnected.
- Set the _LEVEL_ knob to the desired _hold_ voltage.

## <span id="dh">Decay-and-Hold</span> Envelopes

This is very useful for plucks:

<img src="dh.png" height="360" alt="Decay and Hold Envelope" />

- Connect a trigger source to the *TRIG* input.
- Connect a voltage source to the _IN_ input
    to supply the envelope's starting voltage.
- Set the _LEVEL_ knob to the desired _hold_ voltage.

**Note:**
It may be useful to use the trigger signal
to supply the envelope's starting voltage,
as we've done in this example.
