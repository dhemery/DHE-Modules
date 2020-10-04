---
title: The Sequencizer User Guide
---
<img class="faceplate" src="{{'/modules/sequencizer/sequencizer.svg' | relative_url}}" alt="The Sequencizer 4 Faceplate" />


## Step Techniques

## Sequence Techniques

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
