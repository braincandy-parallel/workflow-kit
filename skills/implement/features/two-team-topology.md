---
feature: two-team-topology
status: experimental
modifies:
  - SKILL.md:dispatch
  - SKILL.md:phase-transitions
requires: []
conflicts: []
default_params:
  sub_team_name: <derived>
  max_handoff_depth: 2
source_retro: ""
---

# Two-Team Topology

## What it does

At sprint start, the coordinator spawns a second sub-coordinator with its own team. The main team handles implementation; the sub-team handles spec drafting, design exploration, research, and SPC artifact production. The two teams overlap in wall-clock and coordinate via cross-team SendMessage.

`sub_team_name` defaults to a derived name based on the sprint context. `max_handoff_depth` caps how many round trips a single artifact can bounce between teams before the coordinator forces a decision; default 2 keeps handoff chains short.

## Activation

At sprint start, when the sprint scope includes BOTH:

- An implementation track with workers and a buildable plan, AND
- Three or more SPC / research / design artifacts that need drafting in the same window.

If the sprint is implementation-only, or if the artifact track is too thin to justify a second team, the coordinator does not activate this feature and runs a single team.

## Modification: SKILL.md:dispatch

Spawns a secondary coordinator alongside the primary at sprint start. The secondary coordinator is given:

1. The artifact backlog (list of SPCs / research items to draft).
2. The `sub_team_name`.
3. The cross-team coordination contract (handoff format, target inbox).

## Modification: SKILL.md:phase-transitions

Adds a cross-team coordination contract. At each phase gate, the primary coordinator and sub-coordinator exchange a status message describing artifacts produced, blockers, and any handoffs that exceed `max_handoff_depth`. The primary coordinator owns the sprint timeline; the sub-coordinator reports against it.

## Escape valve

Collapse to a single team. The sub-team finishes its current artifact, posts a final handoff, and shuts down. The main team absorbs any remaining artifact tasks into its own backlog and runs them after implementation winds down. No state migration is needed beyond the handoff messages already in the message log.

## Expected delta vs not-having-it

Spec drafting and implementation overlap fully. On a Phase 2-scale sprint, this is roughly 30K words of SPC draft produced in the same wall-clock window as a full implementation pass. Highest operational complexity in the experimental set: cross-team handoff failures, sub-team drift, and coordinator coordination overhead all become possible failure modes. Composes well with `multi-surface-parallelism` for very large sprints where both axes of parallelism are warranted.
