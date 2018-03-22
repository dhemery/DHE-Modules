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

- [Attack-Release (AR) Envelopes](#ar)
    using _Stage_ modules
- [Attack-Hold-Release (AHR) Envelopes](#hold)
    and Attack-Decay-Hold-Relase (ADHR) envelopes
    using _Hostage_ to generate a _hold_ stage
- [Attack-Decay-Sustain-Release (ADSR) Evelopes](#sustain)
    using _Hostage_ to generate a _sustain_ stage
- [Non-Interruptible Envelopes](#retriggers)
    using _Upstage_ to suppress retriggers
- [Looping Envelopes](#loops)
    using _Upstage_ or _Booster Stage_
    to start the loop
- [Variations](#variations)

## <span id="wiring">Wiring</span> a Multi-Stage Envelope

The general scheme for wiring these modules
into multi-stage envelopes is:

> Place the modules side-by-side
> and connect each output
> to the neighboring input.

Suppose you want generate a four-stage envelope using _Stage_ modules:

<img src="disconnected.png" height="360" alt="Four Stage Modules" />

To wire the modules:

- Connect each module's _ACTIVE_ output
    to its neighbor's _DEFER_ input.
- Connect each module's _EOC_ output
    to its neighbor's _TRIG_ input.
- Connect each module's _OUT_ output
    to its neighbor's _IN_ input.

Here is the result:

<img src="connected.png" height="360" alt="Four Connected Stage Modules" />

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


## <span id="ar">Attack-Release</span> Envelopes

- [Connect](#wiring):
    1. A _Stage_ for the attack stage.
    1. A _Stage_ for the release stage.
- To start the envelope at 0V,
    leave the attack _Stage's_ _IN_ input disconnected.
- Adjust the attack _Stage's_ _LEVEL_
    to set the peak attack voltage.
- Set the release _Stage's_ _LEVEL_ to 0V.

<img src="ar.png" height="360" alt="Attack-Release Envelope" />

## <span id="hold">Attack-Hold-Release<span> Envelopes

- [Connect](#wiring):
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

<img src="ahr.png" height="360" alt="Attack-Hold-Release Envelope" />

To generate an Attack-Decay-Hold-Release (ADHR) envelope,
and insert another _Stage_ before the _Hostage:_

<img src="adhr.png" height="360" alt="Attack-Decay-Hold-Release Envelope" />

## <span id="sustain">Attack-Decay-Sustain-Release<span> (ADSR) Envelopes

To generate a sustain stage,
we need to wire modules a little differently than normal.
We'll start with the usual wiring scheme,
then adjust (see the **bold** text below):

- [Connect](#wiring):
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

<img src="adsr.png" height="360" alt="Attack-Decay-Sustain-Release Envelope" />

## <span id="retriggers">Non-Interruptible Envelopes</span>

Each multi-stage envelope shown above is _retriggerable._
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

![Non-Interruptible Envelope](non-interruptible.png)

Now,
whenever an envelope is in progress,
the final _Stage's_ _ACTIVE_ signal
tells _Upstage_ to suppress incoming triggers.
When the envelope finishes,
_Upstage_ resumes forwarding incoming triggers to the first _Stage._

## <span id="loops">Looping</span> Envelopes

To generate a looping envelope,
we need to
- Manually trigger the first envelope
- Configure the final stage
    to retrigger the envelope
    when it finishes

Two modules offer buttons to manually trigger envelopes:
_Upstage_ and _Booster Stage._

### <span id="upstage-loops">Using</span> _Upstage_ to Create Self-Looping Envelopes

- [Configure](#wiring) a non-looping envelope in the usual way.
- Add an _Upstage_ before the first stage.
- Connect the final stage's _EOC_ output to _Upstage's_ _TRIG_ input.
- Connect _Upstage's_ _TRIG_ output to the first stage's _TRIG_ input.

![Looping with Upstage](loop-upstage.png)

Now you can use _Upstage's_ buttons
to start, stop, and retrigger the loop.

- **To start the loop:**
    Press the _TRIG_ button.
- **To retrigger the loop:**
    Press the _TRIG_ button
    while the envelope is running.
- **To stop the loop:**
    Press the _WAIT_ button
    and hold it
    until the final stage finishes.

**Weird Fact:**
You an also stop the loop
by pressing the _TRIG_ button
(causing the loop to restart)
and holding it until the final stage finishes.

### Using _Booster Stage_ to Create Self-Looping Envelopes

- [Configure](#wiring) a non-looping envelope in the usual way,
    **using a _Booster Stage_ as the first stage.**
- Connect the final stage's _EOC_ output to _Booster Stage's_ _TRIG_ input.

![Looping with Booster Stage](loop-booster-stage.png)

Now you can use _Booster Stage's_ buttons
to start, stop, and retrigger the loop.

- **To start the loop:**
    Press the _TRIG_ button.
- **To retrigger the loop:**
    Press the _TRIG_ button
    while the envelope is running.
- **To stop the loop:**
    Press and hold any _Booster Stage_ button
    until the loop stops.

I leave it as an exercise for the reader
to discover
when and at what voltage
each button stops the envelope.

If you want a gentler, more controlled stop,
[use _Upstage_ and its _WAIT_ button to manage the loop](#upstage-loops).

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
- Follow Artur Karlov's lead and
    [make a sick kick](https://www.youtube.com/watch?v=jVLEKn55MGg).
- Generate audible waveforms
    by [configuring a loop](#loops)
    with very short durations.
- Generate an envelope with seventeen stages.
- Configure a stage to trigger two or more "downstream" stages.
- Vary something.
- Vary something else.
