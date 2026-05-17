---
feature: design-pass-on-first-page
status: experimental
modifies:
  - references/worker.md:dispatch-template
  - SKILL.md:worker-dispatch
requires:
  - locked-design-tokens
conflicts: []
default_params:
  design_profile: null
source_retro: ""
---

# Design Pass On First Page

## What it does

The first worker that builds a user-facing page invokes `/frontend-design` at the start of its session, before any functional implementation. Visual polish, layout primitives, spacing, typography, and component shape are baked into the initial render instead of being applied after the fact by a follow-up polish worker.

`design_profile` defaults to null, leaving the worker to pick the appropriate profile based on the page being built. If the coordinator wants to lock a specific profile, it can override via `default_params`.

## Activation

At the first page-rendering worker dispatch in a sprint. Subsequent page workers in the same sprint inherit the design language established on the first page and do not re-invoke `/frontend-design` unless the coordinator explicitly resets the design context.

## Modification: references/worker.md:dispatch-template

Adds a `/frontend-design` invocation step at the top of the worker brief for the first page-rendering worker. The worker is instructed to:

1. Invoke `/frontend-design` with the page goal.
2. Use the produced design output as the visual contract for the build.
3. Implement functionality on top of the polished shell rather than implementing bare functionality and waiting for a polish pass.

## Modification: SKILL.md:worker-dispatch

Removes the separate polish batch from the default dispatch sequence when this feature is active. Polish is no longer a downstream stage; it is upstream of the first render.

## Escape valve

If the integrated design pass produces conflicts with the functional implementation (e.g., the design draft assumes data shape the API does not support, or the worker cannot reconcile design constraints with the spec), the worker reports the conflict and the coordinator dispatches a follow-up polish worker. This reverts to the baseline behavior at the cost of the time the first worker spent on the integrated pass.

## Expected delta vs not-having-it

Skips a sequential round trip: merge functional implementation, restart dev server, eyeball the bare page, dispatch polish worker, re-merge, restart again. On a Phase 2-sized sprint, this round trip cost roughly 20 minutes. Doing the design pass up front folds those steps into the worker's first session.
