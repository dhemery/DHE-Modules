---
title: Generating Multi-Stage Envelopes
---

Combine
[Stage]({{ '/modules/stage/' | relative_url }}),
[Booster Stage]({{ '/modules/booster-stage/' | relative_url }}),
[Hostage]({{ '/modules/hostage/' | relative_url }}),
and [Upstage]({{ '/modules/upstage/' | relative_url }})
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

## Wiring a Multi-Stage Envelope

The general pattern for wiring multi-stage envelopes is:

> Place the modules side-by-side
> and connect the neighboring ports.

Suppose you want generate a four-stage envelope using _Stage_ modules:

![Four Disconnected Stage Modules](four-disconnected-stage-modules.png)

To wire the modules:

- Connect each module's _ACTIVE_ output
    to the next module's _DEFER_ input.
- Connect each module's _EOC_ output
    to the next module's _TRIG_ input.
- Connect each module's _OUT_ output
    to the next module's _IN_ input.

Here is the result:

![Four Connected Stage Modules](four-connected-stage-modules.png)

Now all that remains is to
connect a trigger source to the first module,
connect the final module's output
to whatever parameter you want your envelope to modulate,
and adjust the levels, curves, and durations to taste.

If you want to know more about how this wiring works,
see
[How Stages Work Together]({{ 'technical/how-stages-work-together' | relative_url }}).

**Note:**
_Upstage_ and _Hostage_
offer slightly different sets of ports.
Still, their ports are arranged so that the
"neighboring ports" connection procedure
usually applies.
See the examples below for details and exceptions.

## Attack-Release Envelopes

## Adding a Hold Stage

## Adding a Sustain Stage

## Preventing Retriggers

## Creating Loops
