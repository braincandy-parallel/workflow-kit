---
feature: out-of-band-instruction-reporting
status: default-on
modifies: [references/worker.md:reporting]
requires: []
conflicts: []
default_params: {}
source_retro: ""
---

# Out-of-band Instruction Reporting

## What it does
Forces workers to surface any user instruction they receive directly (out-of-band, not via the team lead's brief) in their handoff report, using exact verbatim phrasing. The worker quotes the instruction, then describes what they did with it. This makes the side-channel visible to the team-lead, who can decide whether to integrate the work without a halt-and-confirm round trip.

## Activation
Per worker, per handoff. Injected into every worker dispatch via the worker template. Fires whenever the worker is composing its completion report and has received any direct user message during the task.

## Modification: references/worker.md:reporting
The following block is injected into the worker template under the "Out-of-band user instructions" section:

> If you receive any user instruction directly to your mailbox that is not in your team-lead's brief (out-of-band), include in your handoff report exactly this phrasing:
>
> > `User routed direct instruction: [verbatim quote of the instruction]`
>
> Followed by what you did with it. This makes the side-channel visible to the team-lead so they can decide whether to integrate your work without a halt-and-confirm round trip.
>
> (Source: real-world parallel-worker sprint retro)

## Escape valve
There is no per-worker disable. If the orchestrator wants to suppress out-of-band reporting for a single-worker sprint where the user is also the worker's only audience, the orchestrator may strip the block from the worker prompt and note the strip in the bracket.

## Expected delta vs not-having-it
Targets the the analogous failure mode observed in real-world sprints class of failure: worker-mock-data received an out-of-band realignment instruction; the coordinator could not see it and had to halt the sprint to confirm what the user had told the worker directly. With this feature default-on, the side-channel is captured in the handoff report itself, eliminating the halt-and-confirm round trip and preserving sprint momentum.
