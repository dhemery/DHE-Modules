---
title: The Sequencizer Modules
---
<img class="faceplate" src="sequencizer.svg" alt="The Sequencizer 4 Faceplate" />

Generates a sequence of curves.

DHE Modules includes three _Sequencizer_ modules:
- _Sequencizer 4_ (shown).
- _Sequencizer 8._
- _Sequencizer 16._

The three modules operate identically, except for the number of steps.

See:
- [Factory Presets](#factory-presets)
    for descriptions of the presets included with the modules.
- [Usage Notes](#usage-notes)
    for basic ideas about how to use these modules.
- [Details](#details)
    for explanations of several non-intuitive aspects of these modules.
- [J and S Curves]({{ '/technical/curves/' | relative_url }})

## Controls

Each _Sequencizer_ has

- Controls for [the overall sequence](#sequence-controls)
- Controls for [each step in the sequence](#step-controls)
- Controls that [apply to all steps](#global-step-controls)

### Sequence Controls
- **RUN:**
    Determines whether the sequencer is running.

    If _RUN_ falls
    while a sequence is in progress,
    the sequencer pauses the sequence.
    If _RUN_ rises while a sequence is paused,
    the sequencer resumes the sequence.

- **LOOP:**
    If _LOOP_ is on when a sequence ends,
    the sequencer immediately begins a new sequence.
   
    If _LOOP_ is off when a sequence ends,
    the sequencer becomes idle.

- **START** and **LENGTH**
    select the steps of the sequence.
    
    Bracket-shaped markers next to the _ACTIVITY_ lights
    indicate the currently selected steps.   

- **GATE:**
    Controls when a sequence starts,
    and may influence when the sequence advances.

    If _GATE_ rises
    while the sequencer is idle,
    the sequencer starts the sequence,
    activating the first selected, enabled step.

    See the _ADVANCE_ step controls
    for details of how an individual step
    may react to _GATE_ conditions.
    
- **RESET:**
    When _RESET_ rises, the sequencer becomes idle,
    abandoning any paused or in-progress sequence.

    See [details](#details)
    for explanations of several likely non-intuitive aspects of the _RESET_ feature.

- **IN:**
    The input signal to the Sequencizer.

- **OUT:**
    The output signal from the Sequencizer.

### Step Controls

#### Activity Lights

- **ACTIVITY** lights
    indicate which step is active (if any),
    and how far the active step has progressed
    in generating its curve.
    The light progresses from red to green
    over the curve's duration.
    Note that the light indicates *progress,*
    not *voltage.*

#### Advancement Controls

The _TRIG,_ _INT,_ and _AT END_ controls
determine when the sequencer advances
from this step to the next.

- **TRIG:**
    Determines when the step generates a trigger.

    - _RISE:_
        Triggers when the _GATE_ rises.

    - _FALL:_
        Triggers when the _GATE_ falls.

    - _EDGE:_
        Triggers when the _GATE_ rises or falls.

    - _HIGH:_
        Triggers when the _GATE_ is high.

    - _LOW:_
        Triggers when the _GATE_ is low.

- **INT:**
    Determines whether this step
    is interrupted by triggers
    that occur while it is generating a curve.

    - _IGNOR:_
        The step ignores all triggers
        while it is generating a curve.

    - _NEXT:_
        If a trigger occurs
        while the step is generating a curve,
        the sequence advances to the next step.

- **AT END:**
    Determines whether the sequence
    automatically advances to the next step
    when this step completes its curve.

    - _SUST:_
        When the step completes a curve,
        it remains at its _END_ anchor
        until a trigger occurs.

    - _NEXT:_
        When the step completes a curve,
        the sequence automatically advances to the next step.

#### Anchor Controls

The step generates a curve
that interpolates between the voltages
emitted by the _START_ and _END_ anchors.
Each anchor has three controls:

- **MODE** (the unlabeled button above the knob):
    Determines whether the anchor emits
    a sampled voltage or the current voltage.

    - _SAMPL:_
        Emits the voltage sampled from the source
        when the step started.

    - _TRACK:_
        Emits the current voltage of the source.

- **LEVEL** (the unlabeled knob):
    The voltage of the anchor
    when its source is _LEVEL_.
    The range of each _LEVEL_ knob
    is set by the common **LEVEL RANGE** switch
    (to the right of anchors).

- **SOURCE** (the unlabled button below the knob):
    Determines where the anchor gets its voltage.
    The possible sources are:

    - The _LEVEL_ knob.
    - The _IN_ port.
    - The _OUT_ port.
    - The _AUX_ port.     

#### Curve Controls

- **SHAPE:**
    Selects the shape of the curve.

- **CURVE:**
    The curvature of the curve.
    
- **DURATION:**
    The duration of the curve.
    The range of each _DURATION_ knob
    is set by the common **DURATION RANGE** switch
    (to the right of the _DURATION_ knobs),
    modulated by
    the voltage of the duration **CV** port.

#### Enablement Controls

- **ENABLED:**
    Determines whether the sequencer can enter this step.
    When _ENABLED_ is off,
    the sequencer bypasses this step when advancing.

### Global Step Controls

## Factory Presets

The _Sequencizer 4_ module
includes these factory presets:

- **AD:**
    Generates an Attack Decay envelope.
- **ADHR:**
    Generates an Attack Decay Hold Release envelope.
- **ADSR:**
    Generates an Attack Decay Sustain Release envelope.
- **AHR:**
    Generates an Attack Hold Release envelope.
- **ASR:**
    Generates an Attack Sustain Release envelope.
- **Batman:**
    Calls for help.

## Usage Notes

**To create a traditional sequencer step.**

- Set _TRIG_ to _RISE._
- Set _INT_ and _AT END_ to _NEXT._
- Set the _START_ and _END_ anchors
    to _SAMPL_ the _LEVEL_ knob.

**To restart a sequence.**
To interrupt a sequence
and start a new sequence,
send a rising edge to both _RESET_ and _GATE._
The rising _RESET_ resets the sequencer to idle.
The rising _GATE_ immediately starts the next sequence.

**To skip a series of steps.**
Set a series of adjacent steps to advance on the same _GATE_ value
--
either _HIGH_ or _LOW_.
If the selected value appears at the _GATE_
while any of those steps is active,
the sequencer immediately completes the active step
and all of the successive steps with the same _ADVANCE_ condition,
and executes the next available step that has a different _ADVANCE_ condition.

For example, to generate an ADSR envelope
that jumps to the Release step when the _GATE_ goes low,
set the _ADVANCE_ condition
of the Attack, Decay, and Sustain steps
to _LOW._

**Pausing and resuming a sequence.**
Turning _RUN_ off
while a sequence is in progress
_pauses_ the sequence.
When _RUN_ turns on,
the sequence resumes from the point where it was paused.

## Details

Several aspects of the _Sequencizer_ modules,
though purposefully designed,
may be unexpected:

- Each anchor samples its source
    whenever the step starts,
    and only when the step starts.
    While the step is in progress,
    the sampled voltage
    is not affected in any way
    by changes to the anchor's settings
    or by changes in any source's voltage.

- **Once started, a step always completes.**
    Once a step starts,
    it always proceeds as if enabled,
    even if _ENABLED_ is turned off while the step is in progress.    

- **_RESET_ and _RUN._**
    A rising _RESET_
    resets the sequencer to idle
    even while _RUN_ is off.
    If _RUN_ is on,
    a rising _RESET_
    abandons any sequence in progress.
    If _RUN_ is off,
    a rising _RESET_
    abandons any paused sequence.

- **_RESET,_ idle mode, and the _IN_ port.**
    While the sequencer is idle
    and _RESET_ is high,
    the sequencer copies the _IN_ port voltage
    to the _OUT_ port.
