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

- [Wiring a Multi-Stage Envelope](#wiring)
    describes the usual way
    to wire modules together.
- [Attack-Release (AR) Envelopes](#ar)
    using _Stage_ modules.
- [Add a Hold Stage](#hold) using _Hostage._
- [Attack-Hold-Release (AHR) Envelopes](#ahr).
- [Attack-Decay-Hold-Relase (ADHR) Envelopes](#adhr).
- [Add a Sustain Stage](#sustain) using _Hostage._
- [Attack-Decay-Sustain-Release (ADSR) Evelopes](#adsr).
- [Non-Interruptible Envelopes](#retriggers)
    using _Upstage_ to suppress retriggers.
- [Looping Envelopes](#loops)
    using _Upstage_ or _Booster Stage_
    to start the loop.
- [Variations](#variations).

## <span id="wiring">Wiring</span> a Multi-Stage Envelope

The usual way to wire
[_Stage_]({{ '/modules/stage/' | relative_url }}),
[_Booster Stage_]({{ '/modules/booster-stage/' | relative_url }}),
[_Hostage_]({{ '/modules/hostage/' | relative_url }}),
and [_Upstage_]({{ '/modules/upstage/' | relative_url }})
modules
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
connect a trigger source to the first stage,
connect the final stage's output
to whatever parameter you want your envelope to modulate,
and adjust the levels, curves, and durations to taste.

**Note:**
Each module's _LEVEL_ knob
determines the _ending_ level
of the stage it generates.
The starting voltage of each stage
is determined by the signal
arriving at the module's _IN_ input
at the instant the module is triggered.
In this way,
each stage begins
where its upstream neighbor ends.

**Note:**
_Upstage's_ and _Hostage's_ complement of ports
differs slightly from _Stage's._
But even their ports are arranged
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
    leave the attack stage's _IN_ input disconnected.
- To set the peak attack voltage,
    adjust the attack stage's _LEVEL._
- Set the release stage's _LEVEL_ to 0V.

<img src="ar.png" height="360" alt="Attack-Release Envelope" />

## <span id="hold">Add</span> a Hold Stage

The _Hostage_ module is designed specifically
to create hold and sustain stages.

A _hold stage_
holds the envelope voltage constant
_for a given duration._

To generate a sustain stage with _Hostage:_

- Wire it into the chain of modules
    in the position where you want the the hold stage
- Set its mode to _HOLD._

_Hostage_ holds the voltage
at the level where the previous stage ended.
It offers a knob, a range switch, and _CV_ modulation
to set the duration of the hold stage.

## <span id="ahr">Attack-Hold-Release<span> Envelopes

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

## <span id="adhr">Attack-Decay-Hold-Release<span> Envelopes

To generate an Attack-Decay-Hold-Release (ADHR) envelope,
start with an [Attack-Hold-Release envelope](#ahr)
and insert another _Stage_ before the _Hostage:_

<img src="adhr.png" height="360" alt="Attack-Decay-Hold-Release Envelope" />

## <span id="sustain">Add</span> a Sustain Stage

The _Hostage_ module is designed specifically
to create hold and sustain stages.

A _sustain stage_
sustains a constant envelope voltage
_as long as the sustain gate is up._

The wiring for a sustain stage
differs from the usual scheme.
Note the **bold text** below.

To generate a sustain stage with _Hostage:_

- Wire it into the chain of modules
    in the position where you want the the sustain stage.
- Set its mode to _SUSTAIN._
- **Connect its _GATE_ input
    to the same gate source
    that triggers the envelope.**

_Hostage_ sustains the voltage
at the level where the previous stage ended
as long as the gate remains up.

## <span id="adsr">Attack-Decay-Sustain-Release<span> (ADSR) Envelopes

To generate a sustain stage,
we need to wire one module a little differently than normal.
We'll start with the usual wiring scheme,
then adjust (see the **bold** text below):

- [Connect](#wiring):
    1. A _Stage_ for the attack stage.
    1. A _Stage_ for the decay stage.
    1. A _Hostage_ for the sustain stage.
    1. A _Stage_ for the release stage.
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

This example uses an _LFO-1_ to trigger and sustain the envelope.
Note that the envelope uses the LFO's square wave
both to _trigger the envelope_
and to _sustain it._

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
    rather than going directly to the first stage.
- Connect _Upstage's_ _WAIT_ input
    to the final stage's _ACTIVE_ output.

![Non-Interruptible Envelope](non-interruptible.png)

Now,
whenever an envelope is in progress,
the final stage's _ACTIVE_ signal
tells _Upstage_ to suppress incoming triggers.
When the envelope finishes,
_Upstage_ resumes forwarding incoming triggers to the first stage.

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
    replace any _Stage_ module with a
    [_Booster Stage_]({{ '/modules/booster-stage/' | relative_url }}).
- To start each envelope at a level other than 0V,
    connect an
    [_Upstage_]({{ '/modules/upstage/' | relative_url }})
    or other voltage source.
- To start each envelope
    at the level where the previous one ended,
    connect the the final stage's _OUT_ output
    to the first stage's _IN_ input.
- Follow Artur Karlov's lead and
    [make a sick kick](https://www.youtube.com/watch?v=jVLEKn55MGg).
- Generate audible waveforms
    by [configuring a loop](#loops)
    with very short durations.
- Generate an envelope with seventeen stages.
- Configure a stage to trigger two or more "downstream" stages.
- Vary something.
- Vary something else.
