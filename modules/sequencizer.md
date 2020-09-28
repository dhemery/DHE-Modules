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

In addition to its [inputs](#inputs) and [outputs](#outputs),
 each _Sequencizer_ module offers

-   [Sequence controls](#sequence-controls)
    that govern the overall execution of the sequencer. 
-   [Step controls](#step-controls)
    that determine the operation of each step.
-   [Global step controls](#global-step-controls)
    that affect the behavior of all steps.

### Sequence Controls

<span class="controls" style="width: 15mm; height: 77mm; ; background-position: -2.4mm 83mm"></span>

- **RUN**
    determines whether the sequencer is running.

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

- **GATE**
    controls when a sequence starts,
    and may influence when the sequence advances.

    If _GATE_ rises
    while the sequencer is idle,
    the sequencer starts the sequence,
    activating the first selected, enabled step.

    See the [step advancement controls](#step-advancement-controls)
    for details of how an individual step
    may react to _GATE_ conditions.
    
- **RESET:**
    When _RESET_ rises, the sequencer
    abandons any paused or in-progress sequence,
    and either starts a new sequence
    or becomes idle,
    depending on _RUN._

### Step Controls

#### Step Activity Lights
<span class="controls"  style="width: 36mm; height: 9mm; background-position: -56.4mm 91.5mm"></span>

**ACTIVITY** lights
indicate which step is active (if any),
and how far the active step has progressed
in generating its curve.
The light progresses from red to green
over the curve's duration.
Note that the light indicates *progress,*
not *voltage.*

#### Step Advancement Controls
<span class="controls" style="width: 19mm; height: 11mm; background-position: -47mm 83mm"></span>

Three step advancement controls
determine when the sequencer advances
from this step to the next:

- **TRIG**
    determines when the step generates an internal trigger.
    The _INT_ and _SUST_ settings (below)
    determine how the step
    reacts to these internal triggers.

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

- **INT**
    determines whether
    triggers the step
    while it is generating a curve.

    - _NO:_
        The step ignores all triggers
        while it is generating a curve.

    - _YES:_
        If a trigger occurs
        while the step is generating a curve,
        the sequence advances to the next step.

- **SUST**
    determines whether
    the step sustains when it completes its curve.

    - _NO:_
        When the step completes its curve,
        the sequence automatically advances to the next step.

    - _YES:_
        When the step completes its curve,
        it sustains until a trigger occurs.
        While a step sustains,
        it emits the voltage reported by its _END_ anchor.

#### Step Anchor Controls
<span class="controls" style="width: 19mm; height: 33mm; background-position: -47mm 71mm"></span>

The step generates a curve
that interpolates between the voltages
reported by the _START_ and _END_ anchors.
Each anchor has three controls:

- The **MODE** button (above the knob)
    determines whether the anchor reports
    a sampled voltage or the current voltage.

    - _SAMPL:_
        The anchor reports
        the voltage that it sampled from its source
        when the step started.

    - _TRACK:_
        The anchor reports
        the current voltage
        of its source.

- The **LEVEL** knob
    sets the voltage
    of the anchor's _level_ source.
    The [global level controls](#global-level-controls)
    affect the value of this knob.
    This knob is used
    only when _LEVEL_
    is selected as the anchor's _SOURCE._

- The **SOURCE** button (below the knob)
    determines where the anchor gets the voltage it reports.
    The possible sources are:

    - The anchor's _LEVEL_ knob.
    - The _A_, _B_, or _C_ input port.
    - The _OUT_ port.

#### Step Curve Controls
<span class="controls" style="margin-left: 10px; width: 19mm; height: 23mm; background-position: -47mm 38mm"></span>

- **DURATION**
    sets the duration of the step's curve.
    The [global duration controls](#global-duration-controls)
    affect the value of this knob.

- **SHAPE**
    selects the shape of the curve.

- **CURVE**
    sets the curvature of the curve.
    
#### Step Enablement Controls
<span class="controls" style="margin-left: 10px; width: 18mm; height: 7mm; background-position: -47mm 14.5mm"></span>

- **ON**
    determines whether the step is enabled.
    If a step is disabled,
    the sequencer bypasses the step when advancing.

### Global Step Controls

#### Global Level Controls

- **LEVEL**
    <span class="controls" style="margin-left: 10px; width: 27mm; height: 13mm; background-position: -19mm 83.5mm"></span>
    sets a global attenuation factor (0–1)
    applied to every anchor's _LEVEL_ source.
    This knob is modulated
    by the [control voltage](/technical/modulation/) at the _CV_ port.

- **UNI / BI**
    selects the voltage range
    for every anchor _LEVEL_ source.

#### Global Duration Controls

- **DUR**
    <span class="controls" style="margin-left: 10px; width: 27mm; height: 13mm; background-position: -19mm 67mm"></span>
    sets a global multiplier (0–2)
    applied to the duration of each step.
    This knob is modulated
    by the [control voltage](/technical/modulation/) at the _CV_ port.

- **1 / 10 / 100** (the duration range switch)
    selects the global duration range
    for every step duration knob.
    The switch labels indicate the maximum duration (seconds) for each range.
    For each range:

    - The minimum duration is 1/1000 of the maximum: 1ms, 10ms, or 100ms.
    - Centering the step duration knob gives a duration of 1/10 of the maximum: 100ms, 1s, or 10s.

#### Inputs

- **A**, **B**, and **C:**
<span class="controls" style="margin-left: 10px; width: 27mm; height: 13mm; background-position: -19mm 52mm"></span>
    Three input signals
    that each _START_ and _END_ anchor
    can sample or track.

#### Outputs

- **STEP #**
    <span class="controls" style="margin-left: 10px; width: 27mm; height: 13mm; background-position: -19mm 37mm"></span>
    identifies the current step,
    represented as \\(\frac{10s}{N}\\) volts,
    where

    - _s_ is the number of the current step
    - _N_ is the module's largest step number

    For example, if all steps of a _Sequencer 4_
    are selected and enabled,
    and the sequencer is looping,
    the _STEP #_ port will cycle through the values
    
    > 2.5V → 5.0V → 7.5V → 10.0V → 2.5V → …

    When the sequencer is idle,
    the _STEP#_ port emits 0V.

- **CURVE**
    emits 10V if the current step is generating a curve,
    and 0V otherwise.

- **SUST**
    emits 10V if the current step is sustaining,
    and 0V otherwise.

- **STEP ∆**
    <span class="controls" style="margin-left: 10px; width: 27mm; height: 13mm; background-position: -19mm 21mm"></span>
    emits a 10ms pulse
    when a step completes.

- **SEQ ∆**
    emits a 10ms pulse
    when the sequence starts or loops.

- **OUT**
    emits the voltage generated by the current step.

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

### TO DO

- Sample and Hold
- Traditional Sequencer

## Usage Notes

**To create a traditional sequencer step.**

- Set _TRIG_ to _RISE._
- Set _INT_ to _YES._
- Set _SUST_ to _NO._
- Set the _START_ and _END_ anchors
    to _SAMPL_ the _LEVEL_.

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

### Traditional Sequencer

| Parameter | Value |
| TRIG | RISE | Trigger when the GATE rises |
| INT | YES | Advance when the trigger occurs |
| SUST | YES | Sustain until the trigger occurs |
| START anchor | SAMPL LEVEL | Sample and jump to the desired level |
| END anchor | SAMPL LEVEL | Keep the output constant |

### Envelope Generator

| Parameter | Value |
| START anchor | SAMPL OUT | Start where the previous step ended |
| END anchor | SAMPL LEVEL | Progress to the desired level |

### Sample and Hold

| Parameter | Value |
| INT | NO | Ignore triggers while holding |
| SUST | NO | Automatically advance when the hold ends |
| START anchor | SAMPL A | Sample and jump to the input voltage |
| END anchor | SAMPL A | Keep the output constant |

### Sustain Step

| Parameter | Value |
| TRIG | LOW | Trigger if the gate is down |
| INT | YES | Interrupt the curve if triggered |
| SUST | YES | Sustain until triggered | 
| START anchor | SAMPL OUT | Start where the previous step ended | 
| END anchor | SAMPL OUT | Remain where the previous step ended |

### Hold Step

| Parameter | Value |
| INT | NO | Ignore interrupts |
| SUST | NO | Advance when the hold ends |
| START anchor | SAMPL OUT | Start where the previous step ended | 
| END anchor | SAMPL OUT | Remain where the previous step ended |

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
    by changes in parameters or input signals.

-   Once a step starts,
    it always proceeds as if enabled,
    even if it is disabled while the step is in progress.    

-   Pausing and resuming a sequence.
    Turning _RUN_ off
    while a sequence is in progress
    _pauses_ the sequence.
    When _RUN_ turns on,
    the sequence resumes from the point where it was paused.

<script src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.0/MathJax.js?config=TeX-AMS-MML_HTMLorMML" type="text/javascript"></script>

<style type="text/css">
.controls {
  float: right;
  background-image: url('sequencizer.svg');
}
</style>
