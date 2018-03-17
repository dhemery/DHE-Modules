---
title: Generating Multi-Stage Envelopes
---

You can combine the
[Stage]({% link modules/stage.md %}),
[Booster Stage]({% link modules/booster-stage.md %}),
[Hostage]({% link modules/hostage.md %}),
and [Upstage]({% link modules/upstage.md %})
modules to generate complex, interesting, multi-stage envelopes.

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
![A basic chain](basic-chain.png)

This configuration begins each envelope at 0 volts,
and retriggers if a new *TRIG* arrives while the envelope is in progress.

If you want the technical details,
read about [the cascade of events]({% link technical/cascade.md %})

**Variation:**
If you wire the output of a basic chain
to the first Stage's *IN* port.
each envelope after the first will begin
at the voltage
where the previous one left off:

![Continuing Envelope](continuing.png)

**Note that a basic chain is *retriggerable*:**

![A basic chain is retriggerable](retriggerable.png)

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

![Uninterruptible Envelope](uninterruptible.png)

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

![Self-Retriggering Envelope](self-retriggering.png})

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

![Continuing Self-Retriggering Envelope](continuing-self-retriggering.png)
