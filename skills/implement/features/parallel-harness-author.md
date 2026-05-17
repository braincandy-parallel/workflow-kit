---
feature: parallel-harness-author
status: experimental
modifies:
  - SKILL.md:dispatch
requires:
  - stable_acceptance_criteria
conflicts: []
default_params:
  harness_output_path: <plan_dir>/runner.sh
source_retro: ""
---

# Parallel Harness Author

## What it does

When the coordinator dispatches the first implementation worker, it simultaneously dispatches a `harness-author` worker. The harness-author's only job is to draft the acceptance-criteria runner script directly from the PL's acceptance criteria table. It runs in parallel with implementation and does not block any worker batch.

The harness-author writes its output to `<plan_dir>/runner.sh` by default (overridable via `default_params`). When implementation reaches the point where acceptance criteria need exercising, the runner is already drafted.

## Activation

At the first implementation worker dispatch in a sprint. The harness-author is dispatched in the same message as the first implementation worker so the two run concurrently.

## Modification: SKILL.md:dispatch

Adds `harness-author` to the first dispatch batch. The dispatch template gives the harness-author:

1. The path to the PL acceptance criteria table.
2. The target output path (`harness_output_path`).
3. Instructions to draft a script that exercises each acceptance row, without attempting to run it against the in-progress implementation.

`stable_acceptance_criteria` is treated as informational, not enforced by the skill — the coordinator is expected to know whether the PL acceptance section is settled before activating this feature.

## Escape valve

If the PL acceptance criteria change mid-sprint (e.g., a coordinator-side spec revision lands), the harness-author halts its current draft and re-runs after the last change settles. The previously drafted runner is discarded. If acceptance criteria churn repeatedly, the coordinator disables the feature for the remainder of the sprint and reverts to drafting the runner sequentially at the end.

## Expected delta vs not-having-it

Roughly 10 minutes of wall-clock saved by parallelizing what Phase 2 ran sequentially as Batch G (harness drafted after implementation settled). The savings come from overlap, not from cutting any work.
