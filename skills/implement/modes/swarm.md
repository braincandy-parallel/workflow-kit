---
mode: swarm
inherits: turbo
adds: [multi-surface-parallelism, two-team-topology]
removes: []
status: experimental
---

# Mode: Swarm

## Use case
A sprint with ≥3 analogous surfaces to build AND parallel spec/research work in flight. Example: implementing the Fitness, Pharmacology, and Goals & Gates surfaces while drafting their SPCs simultaneously.

Pick this when:
- ≥3 analogous surfaces (page layouts, CRUD modules, dashboards) follow a shared pattern
- A reference implementation of the pattern already exists OR will be built first as the template
- Spec/research work for the same or adjacent surfaces is queued and would otherwise serialize behind implementation
- The team can sustain a second coordinator (sub-coordinator for spec/design alongside the impl coordinator)
- Coordinator-to-coordinator handoff contracts are written before sprint start

## Feature composition
- Inherits from: turbo (which inherits from default, so all 5 default-on features + 3 turbo experimentals are loaded)
- Adds:
  - `multi-surface-parallelism` — derivative surfaces built in parallel against a reference implementation
  - `two-team-topology` — sub-coordinator team for spec/design alongside the impl team
- Removes: (none)
- Conflicts checked:
  - `two-team-topology` interacts with `parallel-harness-author` — both spin up parallel work streams. The loader should confirm the harness author is assigned to the impl team, not the sub-coordinator's team, to avoid two streams editing the same harness.
  - `multi-surface-parallelism` requires the reference implementation to be complete before derivative workers dispatch — the loader should surface this ordering constraint to the PL author.

## Composition warnings
- `multi-surface-parallelism` **requires a reference implementation.** The pattern is: build one surface to completion, then fan out N parallel workers using that surface as the template. If no reference exists at dispatch time, the parallel workers will diverge instead of converging.
- `two-team-topology` **requires a careful coordinator-to-coordinator contract.** Without a written handoff protocol (who owns what artifacts, how spec changes propagate to impl, where conflicts get resolved), the two teams will produce contradictory work and the merge will eat any parallelism gains.
- Inherits turbo's warnings: design-pass-on-first-page needs locked tokens; parallel-harness-author needs stable acceptance criteria.

## Expected scorecard delta vs default mode
- 3 analogous surfaces land in roughly one Phase-2 timeline (down from ~3x serial)
- SPC drafting wall-clock-overlapped with implementation (spec work no longer blocks the impl critical path)
- wall_clock: -50% to -65% on multi-surface sprints (vs default's serial-per-surface pattern)
- preventable_fixes: variable — depends on reference implementation quality; if reference is solid, derivatives inherit its correctness
- coordination_overhead: +1 coordinator role (the sub-coordinator), so net win requires the parallelism payoff to exceed the coordinator tax

## When to NOT pick this mode
- Fewer than 3 analogous surfaces — the multi-surface parallelism overhead exceeds the payoff
- No reference implementation and no time budgeted to build one first — derivative workers will produce divergent code
- No queued spec/research work that would benefit from running alongside impl — two-team-topology adds coordinator cost with no payoff
- Coordinator-to-coordinator handoff contract not written — two-team-topology will produce merge conflicts that eat the parallelism gains
- Solo or two-worker sprints — the topology assumes enough workers to fill both teams without thrashing
- Inherits all turbo "do not pick" cases: backend-only sprints, mid-negotiation design tokens, unstable acceptance criteria
