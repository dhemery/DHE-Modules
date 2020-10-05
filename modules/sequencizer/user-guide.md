---
title: The Sequencizer User Guide
---

- [How Sequencizer Works](#how-sequencizer-works)
    - [Step Phases: Generate and Sustain](#step-phases-generate-and-sustain)
    - [Step Anchors: Start and End Voltages](#anchors-step-start-and-end-voltages)
    - [Advancing from One Step to the Next](#advancing-from-one-step-to-the-next)
- [Generating Constant Voltages](#generating-constant-voltages)
    - [Step Sequencers](#step-sequencers)
    - [Sample and Hold](#sample-and-hold)
- [Generating Envelopes](#generating-envelopes)
    - [Envelope Ramps and Curves](#envelope-ramps-and-curves)
    - [Envelope Ramps and Curves: Variations](#envelope-ramps-and-curves-variations)
    - [Envelope Delay, Hold, and Sustain Steps](#envelope-delay-hold-and-sustain-steps)
- [Morphing Between Signals](#morphing-between-signals)

- [Controlling Sequences](#controlling-sequences)
    - [Retriggering Envelopes](#retriggering-envelopes)
    - [Preventing Interrupts](#preventing-interrupts)
    - [Skipping to the Release Stage](#skipping-to-the-release-stage)
    - [Pausing and Resuming Sequences](#pausing-and-resuming-sequences)

## How Sequencizer Works

### Step Phases: Generate and Sustain

1. **Generate Phase.**
    Each step starts by generating a signal
    that progresses
    from the _start_ anchor voltage
    to the _end_ anchor voltage.
    The generate phase always occurs,
    but may be interrupted
    by a trigger,
    depending on the _INT_ control.
1. **Sustain Phase.**
    When a step finishes generating,
    it optionally sustains
    at the _end_ anchor voltage
    until a trigger occurs.
    The _SUST_ control
    determines whether a step
    executes its sustain phase.

### Step Anchors: Start and End Voltages

A step _generates_
by interpolating
between the voltages
of its _start_ and _end_ anchors
over its duration.
At any moment during the generate phase,
the step emits a voltage
that is part-way between the voltages
reported by these anchors.

An anchor's voltage
is determined by its _source_ and its _mode._

**Anchor Sources.**
An anchor can acquire its voltage from several sources,
selected by the _SOURCE_ control
(the unnamed button above the anchor's knob):
- The _A_, _B_, or _C_ input port
- The _LEVEL_ controls
- The _OUT_ port
    (that is, _Sequencizer's_ previous output voltage)

**Anchor Modes.**
The _MODE_ control
(the unnamed button below the anchor's knob)
determines _when_ the anchor acquires its voltage.

In _SMPL_ mode,
the anchor _samples_ its source
exactly once,
at the start of the step,
and always reports
this sampled voltage.
Any changes in the source voltage
are ignored by the anchor
while the step generates.

In _TRACK_ mode,
the anchor _tracks_ its source,
and always reports
the source's current voltage.

### Advancing from One Step to the Next

The _advancement_ controls
determine when the sequence advances
from one step to the next.

**Triggers.**
While a step is active
(whether generating or sustaining),
it watches the state of the _GATE_,
and generates an internal trigger
whenever the _GATE_
satisfies the condition
specified by the _TRIG_ setting
(rise, fall, change, low, or high).
The next two settings specify
how the step responds to triggers
during each phase.

**Interrupting the Generate Phase.**
The _INT_ control
specifies whether triggers interrupt the step.
If _INT_ is _ON_,
a trigger that occurs while the step is generating

- interrupts the generate phase
- bypasses the step's sustain phase
- advances to the next step

If _INT_ is _OFF_,
the step ignores triggers during its generate phase.

**Sustaining.**
When a step finishes generating,
it optionally sustains
at the _end_ anchor voltage
until a trigger occurs.
If _SUST_ is _ON_
when a step finishes generating,
the step sustains until a trigger occurs.
While it sustains,
it emits its _end_ anchor voltage.
If _SUST_ is _OFF_,
the step skips its sustain phase,
and the sequence immediately advances to the next step.


## Generating Constant Voltages

Many kinds of steps emit constant voltages:
- [Step sequencer](#step-sequencers)
- [Sample and hold](#sample-and-hold)
- [Envelope generator delay, hold, and sustain steps](#envelope-delay-hold-and-sustain-steps)
    (in the next section)

To emit a constant voltage throughout a step,
the general technique is:

- Set the *start* anchor to **SMPL** the desired source.
    This starts the step at the desired voltage,
    and holds the start anchor at the sampled voltage
    until the step ends.
- Set the *end* anchor to **TRACK** the **OUT** port.

If my advice for the end anchor surprises you
(it surprised me when I discovered it),
consider:
- If you set the end anchor to track the _OUT_ port,
    then you can experiment with different start sources,
    and the end anchor will automatically pick up the same constant voltage.
- If instead you set the end anchor to match the start anchor,
    then every time you change the start source,
    you would have to start the end source to match.

### Step Sequencers

- To create a traditional,
    **gate-controlled step sequencer**
    that holds each level until the gate rises,
    configure each step like this:

    - Set **start** anchor to **SMPL** the **LEVEL**.
    - Set **start** level to the desired voltage for the step.
    - Set the **end** anchor to **TRACK** the **OUT** port.
    - Set **TRIG** to **RISE**.
        This configures the step
        to advance when the gate rises.
    - Set **INT** to **YES**.
        This causes the step to advance when triggered,
        regardless of the duration.
    - Set **SUST** to **YES**.
        This makes the step wait until the gate rises,
        regardless of the duration.

    The final two settings
    effectively cause the step
    to ignore the step's duration,
    putting advancement
    entirely under the control of the gate.

- To create a **timer-controlled step sequencer**
    that holds each level for a specified duration,
    configure each step like this:

    - Set **start** anchor to **SMPL** the **LEVEL**.
    - Set **start** level to the desired voltage for the step.
    - Set the **end** anchor to **TRACK** the **OUT** port.
    - Set the **DUR** controls to the desired duration.
    - Set **INT** to **NO**.
        This causes the step to ignore triggers
        and complete its specified duration.
    - Set **SUST** to **NO**.
        This makes the step advance automatically
        at the end of the duration.

    The final two settings
    effectively cause the step
    to ignore the gate,
    putting advancement
    entirely under the control of the step durations.

### Sample and Hold

- To create a **gate-controlled sample-and-hold step**
    that samples its input
    and holds the sampled voltage until the gate rises:

    - Set **start** anchor to **SMPL** the desired input
        (**A**, **B**, or **C**).
        This configures the _sample_ part of _sample-and-hold._
    - Set the **end** anchor to **TRACK** the **OUT** port.
    - Set **TRIG** to **RISE**.
        This configures the step
        to advance when the gate rises.
    - Set **INT** to **YES**.
        This causes the step to advance when triggered,
        regardless of the duration.
    - Set **SUST** to **YES**.
        This makes the step wait until the gate rises,
        regardless of the duration.

    The final two settings
    effectively cause the step
    to ignore the step's duration,
    putting advancement
    entirely under the control of the gate.

- To create a **timer-controlled sample-and-hold step**
    that holds each sample for a specified duration:

    - Set **start** anchor to **SMPL** the **LEVEL**.
    - Set **start** level to the desired voltage for the step.
    - Set the **end** anchor to **TRACK** the **OUT** port.
    - Set the **DUR** controls to the desired duration.
    - Set **INT** to **NO**.
        This causes the step to ignore triggers
        and complete its specified duration.
    - Set **SUST** to **NO**.
        This makes the step advance automatically
        at the end of the duration.

    The final two settings
    effectively cause the step
    to ignore the gate,
    putting advancement
    entirely under the control of the step durations.

## Generating Envelopes

### Envelope Ramps and Curves

For a typical envelope step (attack, decay, or release):

- Set the **start** anchor to **SMPL** the **OUT** port.
    This configures the step
    to progress from the voltage
    where the previous step left off.
- Set the **end** anchor to **SMPL** the **LEVEL**.
    This configures the step
    to progress to the specified voltage.
- Set the end **LEVEL** for the step
    - **Attack:** End at 10V.
    - **Decay:** End at the desired sustain voltage.
    - **Release:** End at 0V.
- Set the duration and curvature as desired.
    A curvature of 0
    (the center position of the _CURVE_ knob)
    produces a linear ramp.

### Envelope Ramps and Curves: Variations

- To start an envelope at 0V
    instead of at the previous output voltage,
    set the attack step's start anchor
    to **SMPL** the **LEVEL**,
    and set the start **level** to 0V.

- To make a step **interruptible:**
    - Set **TRIG** to the desired trigger condition.
    - Set **INT** to **ON**.
        This causes triggers to interrupt the step.
- To make a step **uninterruptible:**
    - Set **INT** to **OFF**.
        This configures the step to ignore triggers
        during its generate phase.

    Note that "uninterruptible" means only "not interrupted by triggers."
    A rising _RESET_ will interrupt even an "uninterruptible" step.

- To **combine decay and sustain _in a single step_:**
    - Set **TRIG** to **LOW**.
        This configures the step
        to generate a trigger
        whenever the gate is low.
    - Set **INT** as desired.
        **ON** makes the decay phase interruptible.
        **OFF** makes it uninterruptible.
    - Set **SUST** to **ON**.
        If the _GATE_ is high when the decay finishes,
        the step will sustain until the _GATE_ falls.

### Envelope Delay, Hold, and Sustain Steps

- Set the *start* anchor to **SMPL** the **OUT** port.
    This configures the step to start
    at the voltage where the previous step finished.
- Set the *end* anchor to **TRACK** the **OUT** port.
    This configures the step
    to emit a constant voltage
    for its full duration.

The rest of the settings depend on
how you want to control the step's duration.

- For a traditional **sustain** step:
    - Set **TRIG** to **LOW**.
        This causes the step to generate a trigger
        if the gate is low
        at any point during the step.
    - Set **INT** to **YES**.
        This configures the step to advance
        if the gate is low
        during the generate phase.
    - Set **SUST** to **YES**.
        This configures the step to advance
        if the gate is low
        during the sustain phase.
- For a traditional **delay** or **hold** step with a fixed duration:
    - Set the **DUR** controls as desired.
    - Set **INT** to **NO**.
        This makes the step uninterruptible.
    - Set **SUST** to **NO**.
        This configures the step
        to advance automatically
        when the duration ends.
- To create a step that holds for a minimum duration,
    then sustains until triggered:
    - Set the **DUR** controls to the desired minimum.
    - Set **TRIG** to **LOW**.
        This configures the step to trigger
        when the gate is low.
    - Set **INT** to **NO**.
        This prevents the triggers from interrupting the hold phase,
        so the step will hold for at least the full duration.
    - Set **SUST** to **YES**.
        This configures the step to sustain
        after the duration ends.

## Morphing Between Signals

All of the techniques described above
configure steps to either
generate a constant voltage
or progress from one fixed voltage to another.

But note that each anchor can _TRACK_ a changing signal:
- The **A**, **B**, and **C** input ports.
- The **OUT** port.
- A step's **LEVEL**, attenuated by the level **CV** signal.

This offers some very creative options.
We can configure steps that:
- Progress from a fixed voltage to a changing signal.
    This has the effect of
    **gradually fading in**
    the changing signal.
- Progress from a changing signal to a fixed voltage.
    This has the effect of
    **gradually fading out**
    the changing signal.
- Progress from one changing signal to another.
    This has the effect of
    **morphing,**
    or
    **gradually fading from one signal to the other.**

## Controlling Sequences

### Retriggering Envelopes

To retrigger an envelope
(that is, to interrupt an in-progress envelope and restart it from the beginning):

- Send a rising edge to both _RESET_ and _GATE_ simultaneously.
- The rising _RESET_ interrupts the current step,
    even if it is ignoring the _GATE_.
- Then the rising _GATE_ immediately restarts the sequence.

### Preventing Interrupts

To make an entire envelope uninterruptible,
in every step:

- Set **INT** **OFF**.
    This causes each step to ignore the state of the gate
    while generating.

Note that a rising _RESET_ signal
interrupts even an _uninterruptible_ step.

### Skipping to the Release Step

For every step except the release step:

- Set **TRIG** to **LOW**.
    This configures the step to generate a trigger
    if the gate falls while the step is in progress,
    or
    **if the gate is low when the step starts.**
- Set **INT** to **ON**.
    This configures the step
    to interrupt its generate phase
    when the trigger occurs,
    or
    **to skip the generate phase
    if the trigger occurs when the step starts.**

### Pausing and Resuming Sequences

- To **pause** a sequence,
    turn _RUN_ off.
- To **resume** a paused sequence,
    turn _RUN_ on.
