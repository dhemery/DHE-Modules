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
- [Sequencizer User Guide]({{ '/modules/sequencizer/user-guide/' | relative_url }})
    for ideas about how to use the _Sequencizer_ modules.
- [Factory Presets](#factory-presets)
    for descriptions of the presets included with the modules.
- [J and S Curves]({{ '/technical/curves/' | relative_url }})

## Controls

-   [Sequence controls](#sequence-controls)
-   [Step controls](#step-controls)
-   [Inputs](#inputs)
-   [Outputs](#outputs)

### Sequence Controls

<img class="controls" style="width: 20mm;" src="sequence-controls.png" alt="Sequence Controls" />

- **RUN**
    specifies whether the sequencer is running.

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

    Bracket-shaped markers next to the _PROGRESS_ lights
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

#### Advancement Controls

<img class="controls" style="width: 25mm;" src="advancement-controls.png" alt="Advancement Controls" />
The advancement controls
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
    triggers interrupt the step
    while it is generating a curve.

    - _OFF:_
        The step ignores all triggers
        while it is generating a curve.

    - _ON:_
        Triggers interrupt the curve.
        If a trigger occurs
        while the step is generating a curve,
        the sequence advances to the next step.

- **SUST**
    determines whether
    the step sustains
    when it completes its curve.

    - _OFF:_
        The step does not sustain.
        When the step completes its curve,
        the sequence automatically advances to the next step.

    - _ON:_
        When the step completes its curve,
        it sustains until a trigger occurs.
        While a step sustains,
        it emits the voltage reported by its _END_ anchor.

#### Anchor Controls

<img class="controls" style="width: 25mm;" src="anchor-controls.png" alt="Anchor Controls" />
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

- The anchor **LEVEL** knob
    sets the voltage
    of the anchor's _level_ source.
    The [global level controls](#global-level-controls)
    affect the value of this knob.
    This knob is used
    only when _LEVEL_
    is selected as the anchor's _SOURCE._

- The **SOURCE** button (below the knob)
    specifies where the anchor gets the voltage it reports.
    The possible sources are:

    - The anchor's _LEVEL_ knob.
    - The _A_, _B_, or _C_ input port.
    - The _OUT_ port.

<img class="controls" style="width: 35mm;" src="level-controls.png" alt="Global Level Controls" />

- The global **LEVEL** knob
    sets a global attenuation factor (0–1)
    applied to every anchor's _LEVEL_ source.
    This knob is modulated
    by the [control voltage](/technical/modulation/) at the _CV_ port.

- **UNI / BI**
    selects the voltage range
    for every anchor _LEVEL_ source.

#### Curve Controls

<img class="controls" style="width: 25mm;" src="curve-controls.png" alt="Curve Controls" />

- The step **DUR** knob
    sets the duration of the step's curve.
    The [global duration controls](#global-duration-controls)
    affect the value of this knob.

- **SHAPE**
    selects the shape of the curve
    (J or S).

- **CURVE**
    sets the curvature of the curve.

<img class="controls" style="width: 35mm;" src="duration-controls.png" alt="Global Duration Controls" />

- The global **DUR** knob
    sets a global multiplier (0–2)
    applied to the duration of each step.
    This knob is modulated
    by the [control voltage](/technical/modulation/) at the _CV_ port.

    Note that though the multiplier
    may be as low as 0,
    the actual duration is never reduced
    below the minimum duration
    of the selected range.

- **1 / 10 / 100** (the duration range switch)
    selects the global duration range
    for every step duration knob.
    The switch labels indicate the maximum duration (seconds) for each range.
    For each range:

    - The minimum duration is 1/1000 of the maximum: 1ms, 10ms, or 100ms.
    - Centering the step duration knob gives a duration of 1/10 of the maximum: 100ms, 1s, or 10s.

#### Execution Controls

<img class="controls" style="width: 36mm;" src="progress-lights.png" alt="Progress Lights" />

- **PROGRESS** lights
    indicate which step is active (if any),
    and how far the active step has progressed
    in generating its curve.
    The light progresses from red to green
    over the curve's duration.

    Note that the light indicates *progress,*
    not *voltage.*

<img class="controls" style="width: 25mm;" src="enablement-controls.png" alt="Enablement Controls" />

- **ON** enables or disables the step.
    If a step is disabled,
    the sequencer bypasses the step when advancing.

### Inputs

<img class="controls" style="width: 35mm;" src="inputs.png" alt="Inputs" />

- **A**, **B**, and **C:**
    Three input signals
    that each _START_ and _END_ anchor
    can sample or track.

### Outputs

<img class="controls" style="width: 35mm;" src="step-state-outputs.png" alt="Step State Outputs" />

- **STEP #**
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
    <img class="controls" style="width: 35mm;" src="step-event-outputs.png" alt="Step Event Outputs" />
    emits a 10ms pulse
    when a step completes.

- **SEQ ∆**
    emits a 10ms pulse
    when the sequence starts or loops.

- **OUT**
    emits the voltage generated by the current step.

## Factory Presets

_Sequencizer 4_
includes a suite of
envelope generator presets:

- **AD:**
    Generates an Attack Decay envelope.
- **ADBDSR:**
    Generates an Attack Decay Break Decay Sustain Release envelope.
    The first decay ends at the specified "break" voltage,
    and the sequence immediately continues with the second decay.
- **ADHR:**
    Generates an Attack Decay Hold Release envelope.
- **ADSR:**
    Generates an Attack Decay Sustain Release envelope.
- **ADSR:**
    Generates an Attack Decay Sustain Release envelope.
- **ADSHR:**
    Generates an Attack Decay Sustain Hold Release envelope.
- **AHDSR:**
    Generates an Attack Hold Decay Sustain Release envelope.
- **AHR:**
    Generates an Attack Hold Release envelope.
- **ASR:**
    Generates an Attack Sustain Release envelope.

Each _Sequencizer_ module
includes these factory presets,
which are primarily useful
as starting points
for your own variations:

- **Sample and Hold 100ms:**
    A timed sample and hold sequence.
    Each step holds for 100ms.
- **Sample and Hold Gated:**
    A gate-controlled sample and hold sequence.
    Each step holds until the gate rises.
- **Steps 100ms:**
    A step sequencer with durations controlled by internal timers.
    Each step holds for 100ms.
- **Steps Gated:**
    A traditional, gate-controlled step sequencer.
    Each step holds until the gate rises.

