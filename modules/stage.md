---
title: The Stage Module
---

<img class="panel" src="panel.svg" alt="The Stage Panel" />

*Stage* generates a single-stage envelope.
Combine *Stages*
to generate complex envelopes
with any number of stages.

## Overview

When the signal at its **TRIG** port rises,
*Stage* generates an envelope stage
and sends it to the **OUT** port.

The envelope stage
starts at the voltage at the **IN** port,
and ends at the voltage set by the **LEVEL** knob.
The **DURATION** knob determines how long
the envelope stage takes
to progress from the _IN_ voltage to the _LEVEL_ voltage.

The **CURVE** knob determines the shape of the envelope stage.
At the center position,
the _CURVE_ knob produces a straight line.
Clockwise rotation produces a J-shaped "logarithmic" curve.
Counterclockwise positions produce an inverted curve.

The **DEFER**, **ACTIVE**, and **EOC** ports,
along with the **TRIG** port,
coordinate control among *Stage* modules
in multi-stage envelopes.
See the [controls](#controls) and [operation](#operation)
sections for details.

## Controls

### Knobs

- The **LEVEL** knob
  ranges from 0 to 10 volts.

- The **DURATION** knob
  ranges from 10 milliseconds to 10 seconds,
  with a duration of 1 second at the center position.

- The **CURVE** knob determines the shape of the envelope stage.

  At the center position, the _CURVE_ knob produces a straight line
  from the start voltage to the end voltage.

  Rotating the _CURVE_ knob clockwise produces a shape that,
  in a rising envelope stage,
  appears J-shaped or "logarithmic" on a Scope.
  The curve approaches the _LEVEL_ voltage more slowly at first,
  then more rapidly at the end.

  Rotating the _CURVE_ knob counterclockwise produces an inverted "logarithmic"
  curve that approaches the _LEVEL_ voltage more rapidly at first,
  then more slowly at the end.

  Rotating the _CURVE_ knob further from the center in either direction
  increases the curvature of the envelope stage.
  At extreme rotations, the curvature is severe.

### Ports

- The **IN** port receives an incoming envelope signal
  from an upstream module.

- The **TRIG** port receives incoming trigger signals.

- The **DEFER** port receives a gate signal
  that tells _Stage_ when to _defer to its input._

- The **ACTIVE** port emits a gate signal (0 or 10 volts)
  that indicates when this _Stage_
  wants downstream modules
  to defer to its output.

- The **EOC** port emits a 10 volt, 1 millisecond pulse
  when _Stage_ finishes generating an envelope stage.

- The **OUT** port
  emits _Stage's_ envelope output.

## Operation

The _DEFER_ and _ACTIVE_ ports
allow upstream _Stage_ modules
to control the output of downstream modules.

The _TRIG_ and _EOC_ ports
allow a chain of _Stage_ modules
to sequence their envelope stages.

Typically you will connect _Stage_ modules in this way:
- Connect the upstream _Stage's ACTIVE_ port
  to the downstream _Stage's DEFER_ port.
- Connect the upstream _Stage's EOC_ port
  to the downstream _Stage's TRIG_ port.
- Connect the upstream _Stage's OUT_ port
  to the downstream _Stage's IN_ port.

As each _Stage_ completes its envelope stage,
it emits a pulse at its _EOC_ port.
This triggers the next _Stage_ module
to begin its own envelope stage.

After its envelope stage ends,
_Stage_ continues to emit the voltage of the _LEVEL_ knob.

## Combining Stages to Generate Multi-Stage Envelopes

Four of the ports
coordinate control in a chain of Stage modules.

The **TRIG** and **EOC** ports
allow each Stage to transfer control
to its downstream neighbor.

When a Stage module completes its envelope stage,
it emits a short 10V pulse at its EOC port.
If you connect the EOC port to another Stage's TRIG port,
then whenever this Stage finishes generating,
its EOC pulse will trigger the next Stage to begin
generating its envelope stage.

In this way, Stage modules hand control from one to the next.

The **DEFER** and **ACTIVE** ports
allow upstream Stages to wrest control
from all of the downstream Stages.

While am envelope stage is in progress,
the **ACTIVE** port emits 10V.
