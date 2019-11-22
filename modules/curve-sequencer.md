---
title: The Curve Sequencer Modules
---
<img class="faceplate" src="curve-sequencer.svg" alt="The Curve Sequencer 4 Faceplate" />

Generates a sequence of curves.

DHE Modules includes three _Curve Sequencer_ modules:
- _Curve Sequencer 4_ (shown).
- _Curve Sequencer 8._
- _Curve Sequencer 16._

The three modules operate identically, except for the number of steps.

See:
- [Usage Notes](#usage-notes)
    for basic ideas about how to use these modules.
    
- [Details](#details)
    for explanations of several non-intuitive aspects of these modules.

## Controls

Each _Curve Sequencer_ has controls for the overall sequence
and controls for each step in the sequence. 

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
    the sequencer immediately restarts the sequence.
   
    When _LOOP_ is off when a sequence ends,
    the sequencer becomes idle.

- **START** and **LENGTH**
    select the steps of the sequence.
    
    Bracket-shaped markers next to the _ACTIVITY_ lights
    indicate the currently selected steps.   

- **GATE:**
    Controls the start and advancement of the sequence.
    
    If _GATE_ rises
    while the sequencer is idle,
    the sequencer starts the sequence
    with the first available step.
    
    While the sequence is in progress,
    the active step may observe the state of the _GATE_
    to determine whether to advance
    to the next step.
    (See the _ADVANCE_ step controls.)
    
- **RESET:**
    When _RESET_ rises, the sequencer becomes idle.

    See [details](#details)
    for explanations of several likely non-intuitive aspects of the _RESET_ feature.

- **IN:**
    The input signal to the curve sequencer.

    See [details](#details)
    for explanations of several likely non-intuitive aspects
    of how and when the sequencer reads the _IN_ port.

- **OUT:**
    The output signal from the curve sequencer.

### Step Controls

- **ACTIVITY** lights
    indicate which step is active (if any),
    and how far the active step has progressed.
    When a step starts, its light is red.
    If the step is in _CURVE_ or _HOLD_ mode,
    the light transitions from red to green
    over the step's duration.
    Note that the light indicates *progress,*
    not *voltage.*

- **MODE:**
    Selects what the step generates:

    - _CURVE_ mode
        generates a curve
        controlled by the _LEVEL,_ _SHAPE,_ _CURVE,_ and _DURATION_ controls.
        When the step finishes generating the curve,
        the sequencer advances to the next enabled step.

    - _HOLD_ mode
        sustains the _OUT_ port voltage
        for the duration selected by the _DURATION_ controls.
        When the duration elapses, 
        the sequencer advances to the next enabled step.

    - _SUSTAIN_ mode sustains the _OUT_ port voltage.
        until the _GATE_ event or condition
        selected by the _ADVANCE_ stepper occurs.

- **ADVANCE:**
    Selects the event or condition
    that triggers the sequencer
    to advance from this step to the next.

    - _TIME:_
        The sequencer advances to the next step
        only when this step completes its full duration.
        The state of the _GATE_ is ignored.

    - _RISE:_
        If the _GATE_ rises
        while this step is active,
        the sequencer advances to the next step.

    - _FALL:_
        If the _GATE_ falls
        while this step is active,
        the sequencer advances to the next step.

    - _EDGE:_
        if the _GATE_ rises or falls
        while this step is active,
        the sequencer advances to the next step.

    - _HIGH:_
        If the _GATE_ is high
        while this step is active,
        the sequencer advances to the next step.

    - _LOW:_
        If the _GATE_ is low
        while this step is active,
        the sequencer advances to the next step.

- **LEVEL:**
    Selects the voltage at which the curve ends.
    The range of each _LEVEL_ knob
    is set by the common **LEVEL RANGE** switch
    (to the right of the _LEVEL_ knobs).
    
- **SHAPE:**
    Sets the shape of the curve.

- **CURVE:**
    Sets the curvature of the curve.
    
- **DURATION:**
    Sets the duration of the _CURVE_ or _HOLD_.
    The range of each _DURATION_ knob
    is set by the common **DURATION RANGE** switch
    (to the right of the _DURATION_ knobs).
    
- **ENABLED:**
    Determines whether the sequencer can enter this step.
    When _ENABLED_ is off,
    the sequencer bypasses this step when advancing.

    NOTE:
    The sequencer observes the _ENABLED_ controls
    only when advancing from one step to the next.
    Once a step starts,
    it always proceeds as if enabled,
    even if _ENABLED_ is turned off while the step is in progress.

## Usage Notes

**To restart a sequence in progress.**
Send a rising edge to both _RESET_ and _GATE._
The rising _RESET_ resets the sequencer to idle.
The rising _GATE_ immediately starts the next sequence.

**To generate a standard ADSR envelope.**
- Enable four steps.
- Attack:
  Set the first step to _CURVE_ mode, 
  and configure its level, curve, shape, and duration to generate the Attack stage.
- Decay:
  Set the second step to _CURVE_ mode, 
  and configure its curve, shape, and duration to generate the Decay stage.
  Set its level to define the sustain level.
- Sustain:
  Set the third step to _SUSTAIN_ mode.
- Release:
  Set the fourth step to _CURVE_ mode, 
  and configure its curve, shape, and duration to generate the Release stage.
- Set the the attack, decay, and sustain stages
  to advance on _LOW._
- Set the release step to advance on _TIME_
  (or any other condition other than _LOW_).

If the _GATE_ goes low during the attack, decay, or sustain stage,
the sequencer interrupts the stage,
skips any intervening stages (because they are set to advance when the _GATE_ is low),
and execute the release step. 

**To track the _IN_ voltage.**




## Details

Several non-intuitive aspects of the _Curve Sequencer_ modules
require explanation.

- **_RESET_ and _RUN._**
    A rising _RESET_
    sets the sequencer to idle
    even while _RUN_ is off.

- **Sequence starts at _OUT_ voltage.**
    The sequencer starts each sequence
    at the current _OUT_ port voltage.
    This ensures that each step starts
    at the same voltage where the previous step ended.

- **_RESET,_ idle mode, and the _IN_ port.**
    If _RESET_ is high while the sequencer is idle,
    the sequencer copies the _IN_ port voltage
    to the _OUT_ port.

- **Reading the _IN_ port.**
