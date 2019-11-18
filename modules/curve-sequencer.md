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
    This happens even when _RUN_ is off.

    NOTE: If _RESET_ and _GATE_ rise simultaneously,
    the sequencer becomes idle,
    then immediately restarts.

    NOTE:
    If _RESET_ is true
    while the sequencer is idle,
    the sequencer transfers its _IN_ voltage
    to its _OUT_ port.

- **IN:**
    The input signal to the curve sequencer.
    
    NOTE: The sequencer observes the _IN_ port
    only when/while _RESET_ is true.

- **OUT:**
    The output signal from the curve sequencer.

### Step Controls

- **ACTIVITY** lights
    indicate which step is active.

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

    - _TIME_ advances when the step completes its full duration.
        The state of the _GATE_ is ignored.

    - _RISE_ advances if the _GATE_ rises.

    - _FALL_ advances if the _GATE_ falls.

    - _EDGE_ advances if the _GATE_ rises or falls.

    - _HIGH_ advances if the _GATE_ is high.
        If the _GATE_ is high when the sequence enters this step,
        the sequencer immediately advances to the next step.

    - _LOW_ advances if the _GATE_ is low.
        If the _GATE_ is low when the sequence enters this step,
        the sequencer immediately advances to the next step.

    NOTE:
    If _TIME_ is selected for a step in _SUSTAIN_ mode,
    the sequencer bypasses the step.

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
