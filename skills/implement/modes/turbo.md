---
mode: turbo
inherits: default
adds: [persistent-dev-server-worker, design-pass-on-first-page, parallel-harness-author]
removes: []
status: experimental
---

# Mode: Turbo

## Use case
A sprint that builds 1-2 page surfaces against locked design tokens, where the dev server is queried frequently and acceptance criteria are stable at sprint start.

Pick this when:
- Surface count is small (1-2 pages or components)
- Design tokens and component library are locked before sprint start
- Acceptance criteria are written and stable (not actively being negotiated mid-sprint)
- The dev server will be hit repeatedly for screenshot, smoke, and visual checks
- The first-render quality of the UI matters (landing pages, demo surfaces, marketing pages)

## Feature composition
- Inherits from: default (loads all 5 default-on features)
- Adds:
  - `persistent-dev-server-worker` — dedicated agent owns the dev server for the whole sprint
  - `design-pass-on-first-page` — /frontend-design baked into the initial render path
  - `parallel-harness-author` — acceptance runner authored in parallel with implementation
- Removes: (none)
- Conflicts checked:
  - `persistent-dev-server-worker` vs `dev-server-concurrency-contract` — the persistent worker IS the single owner, so the contract is satisfied by construction (no conflict, but the loader should confirm the contract feature doesn't double-assign ownership).

## Composition warnings
- `design-pass-on-first-page` **requires locked design tokens.** If tokens are still being negotiated, the design pass will paint over decisions that get reverted, producing churn instead of speed.
- `parallel-harness-author` **requires stable acceptance criteria.** If acceptance criteria are still being negotiated mid-sprint, the harness will be authored against a moving target and need to be rewritten.

## Expected scorecard delta vs default mode
- wall_clock: -20% to -30% (parallelized harness + persistent dev server eliminate setup waits)
- preventable_fixes: -1 to -2 (first-pass design quality reduces "looks broken" fix cycles)
- design-iteration round-trips: -1 (initial render is closer to target on first try)
- false_fails: no expected change

## When to NOT pick this mode
- Surface count is ≥3 analogous surfaces (use swarm instead, which inherits turbo and adds multi-surface parallelism)
- Design tokens are still being decided — design-pass-on-first-page will produce rework
- Acceptance criteria are mid-negotiation — parallel-harness-author will produce a stale harness
- The sprint is a backend-only or infrastructure sprint with no rendered UI — design-pass-on-first-page adds no value
- Single-worker sprints where the persistent dev server worker is a coordination tax with no payoff
