---
title: How Stages Work Together
---

The
[Stage]({{ '/modules/stage/' | relative_url }}),
[Booster Stage]({{ '/modules/booster-stage/' | relative_url }}),
and
[Hostage]({{ '/modules/hostage/' | relative_url }})
modules
can be chained together
to create multi-stage envelopes.

For a chain of individual modules
to generate a sequence of stages,
they must coordinate with each other
in two ways:
_Precedence_ and _Sequence._

1. **Precedence:**
    Whenever an "upstream" module is generating a stage,
    the "downstream" modules
    must forward that signal
    all the way through the chain
    to the final _OUT_ output.

    That is, each module has _precedence_
    over all downstream modules.
    When an upstream module wants control,
    each downstream module must **defer**
    to it.

1. **Sequence:**
    As each module completes its stage,
    it must hand off control
    to its downstream neighbor
    to start the next stage.

## Coordinating Precedence

Modules coordinate precedence
through their _DEFER_ and _ACTIVE_ ports.

In a typical configuration,
each module's _ACTIVE_ output
is connected
to its downstream neighbor's _DEFER_ input.
And each module's _OUT_ output
is connected
to its downstream neighbor's _IN_ input.

When a module is generating a stage,
it holds its _ACTIVE_ gate high
and sends the generated stage to its _OUT_ output.

When the next module
receives this high gate signal at its _DEFER_ input,
it _defers_ to its upstream neighbor.
It forwards its _IN_ signal
(the output from the upstream neighbor)
to its own _OUT_ port.

In this way,
the first module's _OUT_ signal
makes its way
to the second module's _OUT_ output.

In addition,
while the second module is deferring,
it holds its own _ACTIVE_ gate high.
This causes the _next_ module to defer,
and the next,
and the next,
all the way to the end of the chain.

Through this chain of _ACTIVE/DEFER_ signals,
the highest precedence module
assumes control
of the entire chain's output.

## Coordinating Sequence

Modules coordinate sequence
through their _TRIG_ and _EOC_ ports.

In a typical configuration,
each module's _EOC_ output
is connected
to its downstream neighbor's _TRIG_ input.

As each module completes its stage,
it sends two important coordination signals:

- It lowers its _ACTIVE_ gate
    which relinquishes its control
    over the downstream modules.
- It sends a short pulse on its _EOC_ output,
    which hands off control
    to the next module.

And the next module reacts:
- Because its _DEFER_ input is now low,
    it stops deferring.
- Because its _TRIG_ input rose (because of the _EOC_ pulse),
    it begins generating its own stage.
- Because it is actively generating a stage,
    it raises its own _ACTIVE_ gate,
    giving _it_ the highest precedence in the chain.
    And so all of its downstream modules now defer to _it._

In this way,
the chain of modules
creates a sequence of stages
and sends each stage
through the final _OUT_ output of the chain.
