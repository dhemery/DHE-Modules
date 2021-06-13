---
title: Generating Multi-Stage Envelopes
---

Combine
[_Stage_]({{ '/modules/stage/' | relative_url }}),
[_Booster Stage_]({{ '/modules/booster-stage/' | relative_url }}),
[_Hostage_]({{ '/modules/hostage/' | relative_url }}),
and [_Upstage_]({{ '/modules/upstage/' | relative_url }})
modules to generate complex, interesting, multi-stage envelopes.

Here are some common envelope types:

- [AR: Attack-Release Envelopes](#ar)
- [AHR: Attack-Hold-Release Envelopes](#ahr)
- [ADHR: Attack-Decay-Hold-Relase Envelopes](#adhr)
- [ADSR: Attack-Decay-Sustain-Release Evelopes](#adsr)
- [More Stages Than You Can Count](#multi-stage)

**For additional ideas:**
see [Techniques and Variations]({{ '/guides/techniques-and-variations/' | relative_url }}),
which describes
how to wire multi-stage envelopes,
how to add hold and sustain stages,
and how to create
retriggerable envelopes,
uninterruptible envelopes,
looping envelopes,
and other interesting variations.

## <span id="ar">AR:</span> Attack-Release Envelopes

<img class="envelope" src="ar.png" alt="Attack-Release Envelope" />

- [Connect]({{ '/guides/techniques-and-variations/#wiring' | relative_url }}):
    1. A _Stage_ for the attack stage
    1. A _Stage_ for the release stage
- To start the envelope at 0V,
    leave the attack stage's _IN_ input disconnected.
- To set the peak attack voltage,
    adjust the attack stage's _LEVEL._
- Set the release stage's _LEVEL_ to 0V.

## <span id="ahr">AHR: <span> Attack-Hold-Release Envelopes

<img class="envelope" src="ahr.png" alt="Attack-Hold-Release Envelope" />

- [Connect]({{ '/guides/techniques-and-variations/#wiring' | relative_url }}):
    1. A _Stage_ for the attack stage
    1. A _Hostage_ for the hold stage
    1. A _Stage_ for the release stage
- Set _Hostage_ to _HOLD_ mode.
- To start the envelope at 0V,
    leave the attack _Stage's_ _IN_ input disconnected.
- Adjust the attack _Stage's_ _LEVEL_
    to set the peak attack voltage.
    _This also sets the hold level._
- Set the release _Stage's_ _LEVEL_ to 0V.

## <span id="adhr">ADHR:<span> Attack-Decay-Hold-Release Envelopes

<img class="envelope" src="adhr.png" alt="Attack-Decay-Hold-Release Envelope" />

To generate an Attack-Decay-Hold-Release (ADHR) envelope,
start with an [Attack-Hold-Release envelope](#ahr)
and insert another _Stage_ before the _Hostage:_

## <span id="adsr">ADSR: <span> Attack-Decay-Sustain-Release Envelopes

<img class="envelope" src="adsr.png" alt="Attack-Decay-Sustain-Release Envelope" />

To generate a sustain stage,
we need to wire one module a little differently than normal.
Note the **bold** text below.

- [Connect]({{ '/guides/techniques-and-variations/#wiring' | relative_url }}):
    1. A _Stage_ for the attack stage
    1. A _Stage_ for the decay stage
    1. A _Hostage_ for the sustain stage
    1. A _Stage_ for the release stage
- **Connect _Hostage's_ _GATE_ input
    to the same gate source
    that triggers the first stage.**
    This configures the gate source
    to both _trigger_ and _sustain_ the envelope.
- Set _Hostage_ to _SUSTAIN_ mode.
- To start the envelope at 0V,
    leave the attack stage's _IN_ input disconnected.
- Adjust the attack stage's _LEVEL_
    to set the peak attack voltage.
- Adjust the decay stage's _LEVEL_
    to set the sustain level.
- Set the release stage's _LEVEL_ to 0V.

Note that the envelope uses the LFO's square wave
both to _trigger the envelope_
and to _sustain it._

## <span id="multi-stage">More</span> Stages Than You Can Count…

… unless you can count to eighteen.

<img class="envelope2" src="multi-stage.png" height="auto" max-width="740" alt="An Eighteen-Stage Envelope" />
