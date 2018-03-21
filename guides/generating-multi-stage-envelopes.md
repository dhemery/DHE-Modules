---
title: Generating Multi-Stage Envelopes
---

Combine
[_Stage_]({{ '/modules/stage/' | relative_url }}),
[_Booster Stage_]({{ '/modules/booster-stage/' | relative_url }}),
[_Hostage_]({{ '/modules/hostage/' | relative_url }}),
and [_Upstage_]({{ '/modules/upstage/' | relative_url }})
modules to generate complex, interesting, multi-stage envelopes.

Here are some of the possibilities:

- [Attack-Release (AR) Envelopes](#attack-release-envelopes)
- [Add a Hold Stage](#add-a-hold-stage)
   to generate an Attack-Hold-Release (AHR)
   or Attack-Decay-Hold-Release (ADHR) envelope.
- [Add a Sustain Stage](#add-a-sustain-stage)
    to generate an Attack-Decay-Sustain-Release (ADSR) envelope.
- [Prevent Retriggers](#prevent-interruption)
    to allow each envelope to run to completion
    without interruption from incoming triggers.
- [Create Loops](#create-loops)
    to generate envelopes that automatically repeat.
- [Variations](#variations)

## Wiring a Multi-Stage Envelope

The general scheme for wiring these modules
into multi-stage envelopes is:

> Place the modules side-by-side
> and connect each output
> to the neighboring input.

Suppose you want generate a four-stage envelope using _Stage_ modules:

![Four Disconnected Stage Modules](four-disconnected-stage-modules.png)

To wire the modules:

- Connect each module's _ACTIVE_ output
    to its neighbor's _DEFER_ input.
- Connect each module's _EOC_ output
    to its neighbor's _TRIG_ or _GATE_ input.
- Connect each module's _OUT_ output
    to its neighbor's _IN_ input.

Here is the result:

![Four Connected Stage Modules](four-connected-stage-modules.png)

Now all that remains is to
connect a trigger source to the first module,
connect the final module's output
to whatever parameter you want your envelope to modulate,
and adjust the levels, curves, and durations to taste.

**Note:**
_Upstage's_ and _Hostage's_ ports
differ slightly from _Stage's._
Still, their ports are arranged
so that the usual wiring scheme —
"connect outputs to neighboring inputs" —
usually applies.
See the examples below for details and exceptions.

**See also:**
[How Stages Work Together]({{ 'technical/how-stages-work-together' | relative_url }}).


## Attack-Release Envelopes

To generate an Attack-Release (AR) envelope:

- [Connect](#wiring-a-multi-stage-envelope):
    1. A _Stage_ for the attack stage.
    1. A _Stage_ for the release stage.
- To start the envelope at 0V,
    leave the attack _Stage's_ _IN_ input disconnected.
- Adjust the attack _Stage's_ _LEVEL_
    to set the peak attack voltage.
- Set the release _Stage's_ _LEVEL_ to 0V.

![Attack-Release Envelope](attack-release-envelope.png)

## Add a Hold Stage

To generate an Attack-Hold-Release (AHR) envelope:

- [Connect](#wiring-a-multi-stage-envelope):
    1. A _Stage_ for the attack stage.
    1. A _Hostage_ for the hold stage.
    1. A _Stage_ for the release stage.
- Set _Hostage_ to _HOLD_ mode.
- To start the envelope at 0V,
    leave the attack _Stage's_ _IN_ input disconnected.
- Adjust the attack _Stage's_ _LEVEL_
    to set the peak attack voltage.
    _This also sets the hold level._
- Set the release _Stage's_ _LEVEL_ to 0V.

![Attack-Hold-Release Envelope](attack-hold-release-envelope.png)

To generate an Attack-Decay-Hold-Release (ADHR) envelope,
insert another _Stage_ before the _Hostage:_

![Attack-Decay-Hold-Release Envelope](attack-decay-hold-release-envelope.png)

## Add a Sustain Stage

To generate an Attack-Decay-Sustain-Release (ADSR) envelope,
we need to wire modules a little differently than normal.
We'll start with the usual wiring scheme,
then adjust (see the **bold** text below):

- [Connect](#wiring-a-multi-stage-envelope):
    1. A _Stage_ for the attack stage.
    1. A _Stage_ for the decay stage.
    1. A _Hostage_ for the hold stage.
    1. A _Stage_ for the release stage.
- **Connect _Hostage's_ _GATE_ input
    to the same gate source
    as the decay _Stage's_ _TRIG_ input.**
    This configures the gate source
    to both _trigger_ and _sustain_ the envelope.
- Set _Hostage_ to _SUSTAIN_ mode.
- To start the envelope at 0V,
    leave the attack _Stage's_ _IN_ input disconnected.
- Adjust the attack _Stage's_ _LEVEL_
    to set the peak attack voltage.
- Adjust the decay _Stage's_ _LEVEL_
    to set the sustain level.
- Set the release _Stage's_ _LEVEL_ to 0V.

![Attack-Decay-Sustain-Release Envelope](attack-decay-sustain-release-envelope.png)

## Prevent Retriggers

Each multi-stage envelope shown above is *retriggerable:*
If a trigger arrives
while an envelope stage is in progress,
the new trigger
**_interrupts the in-progress envelope_**
and begins a new one.

Often that's the behavior you want.

Sometimes it isn't.

If you want to prevent incoming triggers
from interrupting in-progress envelopes:

- Insert an _Upstage_ at the front of the chain.
- Rewire the trigger source
    so that the trigger signal passes through _Upstage_
    rather than going directly to the first _Stage._
- Connect _Upstage's_ _WAIT_ input
    to the final _Stage's_ _ACTIVE_ output.

![Prevent Retriggers](prevent-retriggers.png)

Now,
whenever an envelope is in progress,
the final _Stage's_ _ACTIVE_ signal
tells _Upstage_ to suppress incoming triggers.
When the envelope finishes,
_Upstage_ resumes forwarding incoming triggers to the first _Stage._

## Create Loops

## Variations
- For additional versatility,
    replace any _Stage_ with a
    [_Booster Stage_]({{ '/modules/booster-stage/' | relative_url }}).
- To start each envelope at a level other than 0V,
    connect an
    [_Upstage_]({{ '/modules/upstage/' | relative_url }})
    or other voltage source.
- To start each envelope
    at the level where the previous one ended,
    connect the the final module's _OUT_ output
    to the first module's _IN_ input.
- [Make a sick kick](https://www.youtube.com/watch?v=jVLEKn55MGg).
- Generate audible signals
    by setting _Stage_ and _Booster Stage_
    to very short durations.
- Vary something.
- Vary something else.
