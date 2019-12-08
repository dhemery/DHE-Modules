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
- [Factory Presets](#factory-presets)
    for descriptions of the presets included with the modules.
- [Usage Notes](#usage-notes)
    for basic ideas about how to use these modules.
- [Details](#details)
    for explanations of several non-intuitive aspects of these modules.
- [J and S Curves]({{ '/technical/curves/' | relative_url }})

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
    The input signal to the curve sequencer.

- **OUT:**
    The output signal from the curve sequencer.

### Step Controls

- **ACTIVITY** lights
    indicate which step is active (if any),
    and how far the active step has progressed.
    When a step starts, its light is red.
    If the step is in a timed mode
    (_CURVE, HOLD, INPUT, CHASE,_ or _LEVEL_), 
    the light transitions from red to green
    over the step's duration.
    Note that the light indicates *progress,*
    not *voltage.*

- **MODE:**
    Selects what kind of envelope stage this step generates:

    - _CURVE_ mode
        generates a curve
        ending at the _LEVEL_ voltage.
        The _SHAPE,_ _CURVE,_ and _DURATION_ controls
        govern the shape and duration of the curve.

    - _HOLD_ mode
        holds the _OUT_ port at its current voltage
        for the duration selected by the _DURATION_ controls.

    - _SUSTAIN_ mode
        sustains the _OUT_ port at its current voltage
        until the _GATE_ condition
        selected by the _ADVANCE_ stepper occurs.

    - _INPUT_ mode
        sends the _IN_ signal to the _OUT_ port
        for the duration selected by the _DURATION_ controls.

    - _CHASE_ mode
        generates a curve
        that progressively approaches
        the _IN_ voltage.
        The _SHAPE,_ _CURVE,_ and _DURATION_ controls
        govern the shape and duration of the approach.

    - _LEVEL_ mode
        sends the _LEVEL_ voltage to the _OUT_ port.

- **ADVANCE:**
    Selects the condition
    that triggers the sequencer
    to advance from this step to the next.

    - _TIME:_
        The sequencer advances to the next step
        when this step completes its full duration.
        The state of the _GATE_ is ignored.

        Selecting this option
        for a step in _SUSTAIN_ mode
        disables the step.

    - _RISE:_
        If the _GATE_ rises
        while this step is active,
        the sequencer advances to the next step.

    - _FALL:_
        If the _GATE_ falls
        while this step is active,
        the sequencer advances to the next step.

    - _EDGE:_
        If the _GATE_ rises or falls
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
    The voltage at which the curve ends.
    The range of each _LEVEL_ knob
    is set by the common **LEVEL RANGE** switch
    (to the right of the _LEVEL_ knobs).
    
- **SHAPE:**
    Selects the shape of the curve.

- **CURVE:**
    The curvature of the curve.
    
- **DURATION:**
    The duration of the _CURVE_ or _HOLD_.
    The range of each _DURATION_ knob
    is set by the common **DURATION RANGE** switch
    (to the right of the _DURATION_ knobs).

- **ENABLED:**
    Determines whether the sequencer can enter this step.
    When _ENABLED_ is off,
    the sequencer bypasses this step when advancing.

## Factory Presets

The _Curve Sequencer 4_ module
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
Select _LEVEL_ mode
and set it to _ADVANCE_ on _RISE._
Note that using the other _ADVANCE_ options
give you a great deal of flexibility
compared to a traditional sequencer.

**_INPUT_ mode**
essentially bypasses the sequencer
for the duration of the step.

**_CHASE_ mode**
generates a curve
that chases a moving target:
The voltage at the _IN_ port.

**To restart a sequence.**
To interrupt a sequence
and start a new sequence,
send a rising edge to both _RESET_ and _GATE._
The rising _RESET_ resets the sequencer to idle.
The rising _GATE_ immediately starts the next sequence.

**To start a sequence at a specified voltage.**
- Feed the _IN_ port a signal with the desired voltage.
- Send a rising edge to both _RESET_ and _GATE._
  The rising _RESET_ 
  copies the _IN_ voltage to the _OUT_ port.
  The rising _GATE_ immediately starts a new sequence
  starting from the _OUT_ port voltage.

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

**To track the _IN_ voltage.**
If _RESET_ is high
while the sequencer is idle,
the sequencer copies the _IN_ voltage to the _OUT_ port.
So if you want control the sequencer externally
to "track" the _IN_ signal:

- Send a rising signal to _RESET._
    The sequencer interrupts any sequence in progress and becomes idle.
- Hold _RESET_ high.
    As long as _RESET_ is high
    and the sequencer is idle,
    the sequencer copies _IN_ to _OUT_.
- Prevent _GATE_ from rising.
    If _GATE_ rises while the sequencer is idle,
    the sequencer starts a sequence
    and stops tracking _IN_
    (unless the first selected step is in _INPUT_ mode).
- To stop tracking, set _RESET_ low or send a rising signal to _GATE._

**Pausing and resuming a sequence.**
Turning _RUN_ off
while a sequence is in progress
_pauses_ the sequence.
When _RUN_ turns on,
the sequence resumes from the point where it was paused.

## Details

Several aspects of the _Curve Sequencer_ modules,
though purposefully designed,
may be unexpected:

- **_ADVANCE_ options and _timed_ modes.**
    If the active step is in one of the _timed_ modes
    (_CURVE, HOLD, INPUT, CHASE,_ or _LEVEL_), 
    the sequencer advances to the next step
    as soon as either or both of these things occurs:

    - The active step completes its duration.
    - The _GATE_ satisfies the _ADVANCE_ condition.
    
    If the active step completes its duration,
    the sequencer advances to the next step
    regardless of whether the _GATE_ 
    satisfies the _ADVANCE_ condition.

    If the _GATE_ satisfies the _ADVANCE_ condition,
    the sequencer advances to the next step
    regardless of whether the active step
    regardless of whether the active step
    has completed its duration.

- **The starting voltage of each mode.**
    A step in _CURVE, HOLD, SUSTAIN,_ or _CHASE_ mode
    starts at the current _OUT_ port voltage.

    A step in _INPUT_ mode
    starts at the the voltage at the _IN_ port.
    and tracks the _IN_ signal
    for the duration.

    A step in _LEVEL_ mode
    starts at the voltage specified by the _LEVEL_ controls
    and tracks the value of those controls
    for the duration.

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
