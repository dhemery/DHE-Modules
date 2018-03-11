---
title: The Cascade of Events
---

### The Cascade of Events…

At the start, before the first stage is triggered,
each module sends 0 volts to its output.
At this point, the only one that matters
is the final stage.
Its output is 0 volts.

When stage 1 receives a trigger,
that starts a cascade of events:

- Stage 1 begins an envelope stage.
  Because its *IN* port is disconnected,
  the envelope stage begins at 0 volts.
- Because stage 1 is now active
  (generating an envelope)
  it raises its *ACTIVE* gate.
- This causes Stage 2's *DEFER* gate to goes high.
  So stage 2 begins *deferring.*
  That is,
  it begins forwarding its *IN* port voltage
  directly to its *OUT* port.
- Because stage 2 is now active
  (deferring to its input)
  it raises its *ACTIVE* gate.
- This *ACTIVE/DEFER* signal propagates downstream,
  and stages 3 and 4 begin deferring
  to their immediate upstream neighbors.
  In this way,
  the signal that originates at stage 1
  flows all the way through the chain
  to stage 4's *OUT* port.
- The chain continues to emit the signal from stage 1
  until stage 1's envelope stage ends.
- When stage 1's envelope stage ends,
  lowers its *ACTIVE* gate.
- *At the same time*
  (before any other module has an opportunity to respond)
  stage 1 emits an *end of cycle* pulse at its
  *EOC* port.
- At stage 2,
  the *DEFER* port goes low.
  *At the same time*
  (before it has time to lower its own *ACTIVE* gate)
  it receives a trigger at its *TRIG* port.
- Stage 2 immediately initiates an envelope stage
  starting with the value at its *IN* port.
  That is, the value at which stage 1 ended.
- Stage 2 remains *ACTIVE,*
  and the downstream stages now defer to it.
  Until stage 2 completes,
  its envelope stage flows to the end end of the chain.
- And so on until stage 4 completes.
- After stage 4 completes,
  it continues to emit the value of its *LEVEL* knob
  until it receives the next *DEFER* or *TRIG* signal
  from its upstream neighbor.
- When the next *TRIG* arrives at stage 1,
  it begins an envelope stage
  starting at 0 volts.
- And the cycle repeats…
