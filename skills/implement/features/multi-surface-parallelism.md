---
feature: multi-surface-parallelism
status: experimental
modifies:
  - SKILL.md:dispatch
  - references/worker.md:checklists
requires:
  - locked-design-tokens
  - reference-implementation-complete
conflicts: []
default_params:
  reference_surface: null
source_retro: ""
---

# Multi-Surface Parallelism

## What it does

When a PL describes three or more analogous surfaces (e.g., several derivative pages mirroring a reference page) AND a reference implementation already exists, the coordinator dispatches one worker per surface in parallel worktrees. Each worker reads the reference page, then mirrors the pattern onto its assigned target surface.

Workers do not collaborate during the parallel phase; they each produce an independent worktree. Integration happens at merge time, gated by per-batch verifier checks that compare each surface against the reference shape.

## Activation

When the PL task table has three or more analogous surface tasks explicitly marked as `derivative` or `mirror-of-<ref>`. The coordinator reads the marker, identifies the reference surface (either from the PL or from `default_params.reference_surface`), and dispatches the parallel batch.

If fewer than three derivative surfaces are present, the feature does not fire and the coordinator falls back to sequential dispatch.

## Modification: SKILL.md:dispatch

Adds a parallel worker dispatch branch:

1. Detect derivative-surface count in the PL task table.
2. If ≥3, allocate one worker per surface in its own worktree.
3. Each worker brief points at the reference page and the target surface.
4. Coordinator collects all worktrees and merges in order, running the structural-match check per merge.

## Modification: references/worker.md:checklists

Adds a `structural-match-to-reference` checklist item for any worker assigned a derivative surface. The checklist instructs the worker to confirm component structure, prop shape, and layout primitives match the reference before reporting their pane done.

## Escape valve

The per-batch Verifier runs a "this page matches reference shape" check on each merged surface. On failure, the coordinator pauses any remaining parallel dispatch and switches to sequential merge for the rest of the surfaces, letting each one fix forward against the now-merged set.

`requires` are treated as informational — the coordinator is expected to confirm design tokens are locked and the reference implementation has landed before activating the feature.

## Expected delta vs not-having-it

Three derivative surfaces in roughly one Phase-2 timeline (~7 hours of wall clock instead of ~21 hours sequential). Highest yield in the experimental set and also the highest blast radius: a structural drift in the reference template can propagate across all parallel surfaces before any verifier catches it. Composes well with `persistent-dev-server-worker`, since N parallel surfaces compound the dev-server race risk that feature mitigates.
