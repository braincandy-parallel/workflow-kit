---
feature: pre-flight-readiness-check
status: default-on
modifies: [SKILL.md:dispatch]
requires: []
conflicts: []
default_params: {}
source_retro: ""
---

# Pre-flight Readiness Check

## What it does
Forces the orchestrator to run a 60-second readiness check before launching workers for any batch. The check covers dependency declarations in the relevant `package.json` files, dev-port availability, stale worktrees from prior batches, existence of target files, and explicit ownership of the dev server. If any check fails, the orchestrator addresses it before dispatching workers rather than discovering the failure mid-batch.

## Activation
Per batch dispatch. Runs in the dispatch phase of the implement skill, immediately before workers are launched, every time a new batch begins (including resumed sprints).

## Modification: SKILL.md:dispatch
The following block is injected into SKILL.md under the "Pre-flight Readiness (mandatory before each batch dispatch)" section:

> Before launching workers for a batch, run a 60-second readiness check:
>
> 1. **Dependency declarations.** For each file the batch's workers will modify, are the required workspace deps declared in the appropriate `package.json`? Example: a worker importing from `@<org>/ui` requires `"@<org>/ui": "workspace:*"` declared in the consumer's `package.json`. Missing deps surface as "out-of-surface" worker reports, costing a follow-up commit cycle.
> 2. **Ports free.** Run `lsof -i :3000` (or the project's dev port). Note any existing processes; kill or use a different port for the batch.
> 3. **No stale worktrees.** `git worktree list`. Remove worktrees that match this team's name but are from prior batches.
> 4. **Target files exist.** Quick `ls` on the files each worker is supposed to modify. Missing files mean the brief needs to mention "create the file."
> 5. **Dev server ownership.** Confirm which agent owns the dev server for this batch. Document the ownership in the bracket or in TaskCreate metadata.
>
> If any check fails, address it before dispatching workers.
>
> Source: real-world parallel-worker sprint retro.

## Escape valve
For single-worker dispatches where the readiness check would be more ceremony than the task, the orchestrator may skip the check and note the skip in the bracket. The check is mandatory for any batch of 2+ workers and for any batch touching workspace dependencies, deploy targets, or dev servers.

## Expected delta vs not-having-it
Targets the the analogous failure mode observed in real-world sprints class of failure (missing `@your-org/ui` workspace dep declared post-dispatch, ~10 min/batch lost to follow-up commit cycles) and incident #4 (transient peerDep resolution race). Catching these upfront removes the round-trip cost of an out-of-surface worker report, a coordinator re-dispatch, and a second commit cycle. Expected savings: ~10 min per batch where any dependency or port collision would otherwise be discovered post-dispatch.
