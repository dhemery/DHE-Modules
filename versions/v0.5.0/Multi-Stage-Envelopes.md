Stage modules can be chained
to create multi-stage envelopes.

Here are some of the possibilities:

- [Basic Multi-Stage Envelope](#basic-multi-stage-envelope):
  Create a multi-stage envelope.
- [Preventing Retriggers](#preventing-retriggers):
  Create an envelope that ignores new triggers
  while it is in progress.
- [Self-Retriggering](#self-retriggering):
  Create a multi-stage envelope that
  automatically retriggers itself
  when its last stage ends.

## Basic Multi-Stage Envelope

To build a basic multi-stage envelope:

- Connect a trigger source to the first stage's *TRIG* port.
- Connect each stage's *OUT* port
  to the following stage's *IN* port.
- Connect each stage's *EOC* port
  to the following stage's *TRIG* port.
- Connect each stage's *ACTIVE* port
  to the following stage's *DEFER* port.
- Connect the final stage's *OUT* port
  to the destination for the envelope.

The chain looks like this:
![A basic chain](images/v0.6/basic-chain.png)

This configuration begins each envelope at 0 volts,
and retriggers if a new *TRIG* arrives while the envelope is in progress.

Let's step through the details.

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

**Variation:**
If you wire the output of a basic chain
to the first Stage's *IN* port.
each envelope after the first will begin
at the voltage
where the previous one left off:

![Continuing Envelope](images/v0.6/continuing.png)

**Note that a basic chain is *retriggerable*:**

![A basic chain is retriggerable](images/retriggerable.png)

The green arrow indicates where a new trigger arrives.
The chain starts a new envelope,
interrupting the one already in progress.

## Preventing Retriggers

To create a chain that ignores triggers
while it is generating an envelope:

- Add an [[Upstage]] module.
- Connect the trigger source to Upstage's *TRIG* (trigger in) port.
- Connect Upstage's  *TRIG* (trigger out) port
  to the first Stage's *TRIG* port.
- Connect the final Stage's *ACTIVE* port
  to Upstage's *WAIT* port.

The uninterruptible chain looks like this:

![Uninterruptible Envelope](images/uninterruptible.png)

Here's how it works:
- While the chain is active,
  the final Stage's *ACTIVE* gate is high.
- Upstage's *WAIT* port instructs Upstage
  to ignore incoming triggers.
- So while the chain is active,
  it ignores incoming triggers.

Note:
As the arrows show,
this configuration will often leave a gap
between the end of one envelope (red arrow)
and the start of the next (green arrow).

## Self-Retriggering

To generate a self-retriggering envelope:

- Use a [[Booster Stage]] as the first module in the chain.
- Connect the final Stage's *EOC* port
  to Booster Stage's *TRIG* port.

The self-retriggering chain looks like this:

![Self-Retriggering Envelope](images/v0.6/self-retriggering.png)

NOTE: There is no external trigger source for this chain!

**To start the envelope:**
Press Booster Stage's *TRIG* button.

Here's how it works:
- Pressing Booster Stage's *TRIG* button
  triggers it to begin generating its stage,
  starting an envelope.
- When the envelope ends
  the final Stage emits a brief pulse at its *EOC* port.
- Booster Stage receives that pulse at its *TRIG* port
  and begins generating its stage.

**To stop the envelope:**
Stopping this configuration is either weird or troublesome,
depending on your perspective.
Here are two methods:

- Press Booster Stage's *ACTIVE* button.
  Pressing the *ACTIVE* button
  causes the downstream stages to defer
  to Booster Stage.
  Because Booster Stage is actually inactive,
  it continues to emit whatever voltage
  it was emitting when you released the button.
- Press Booster Stage's *DEFER* button.
  This puts *every* stage
  (including Booster Stage!)
  in DEFERRING mode.
  And every stage emits whatever voltage
  the final stage was emitting
  when you pressed the button.

As I said, weird. Or troublesome. Take your pick.
If you want to be able to stop a self-retriggering envelope
in a more controlled way,
add an Upstage module at the start of the chain.

**Variation:**
If you wire the output of a [self-retriggering chain](#self-retriggering-envelope)
to the Booster Stage's *IN* port.
each envelope after the first will begin
at the voltage
where the previous one left off:

![Continuing Self-Retriggering Envelope](images/v0.6/continuing-self-retriggering.png)
