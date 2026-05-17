---
feature: paired-value-smoke-checks
status: default-on
modifies: [references/worker.md:smoke-checks]
requires: []
conflicts: []
default_params: {}
source_retro: ""
---

# Paired-value Smoke Checks

## What it does
Forces workers running UI smoke checks to grep for ALL expected values on a rendered surface, not just one. When a page renders paired values (weekday/weekend targets, min/max ranges, before/after states), the worker must run a separate grep for each member of the pair. Asymmetric formatting between paired values is a known defect mode that one-sided grep checks miss entirely.

## Activation
Per worker, per smoke check. Injected into every worker dispatch via the worker template. Fires whenever a worker is doing a render-output verification step that involves more than one expected value on the same surface.

## Modification: references/worker.md:smoke-checks
The following block is injected into the worker template under the "Smoke-check paired values symmetrically" section:

> When checking that data renders correctly on a UI surface, grep for ALL expected values, not just one. Example: if a page renders both weekday (1,750) and weekend (2,100) calorie targets, smoke check `grep -c '1,750' /tmp/dp.html` AND `grep -c '2,100' /tmp/dp.html`. Asymmetric formatting between paired values is a common defect mode that one-sided grep checks miss entirely.
>
> (Source: real-world parallel-worker sprint retro)

## Escape valve
For surfaces that genuinely render only a single value, this check collapses to the existing one-grep pattern by default. There is no runtime toggle; the rule is "grep every expected value," which degenerates naturally when there is only one.

## Expected delta vs not-having-it
Targets the the analogous failure mode observed in real-world sprints defect class: 1,750 weekday rendered formatted but 2,100 weekend rendered raw, missed by two workers and caught only by the final verifier. With paired-value grepping at the worker level, formatting asymmetry surfaces inside the worker's own smoke check rather than propagating to the verifier round. Expected delta: per-defect savings of one verifier round-trip, plus one re-dispatch to the original worker.
