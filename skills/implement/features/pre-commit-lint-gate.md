---
feature: pre-commit-lint-gate
status: default-on
modifies: [references/worker.md:smoke-checks]
requires: []
conflicts: []
default_params: {}
source_retro: ""
---

# Pre-commit Lint Gate

## What it does
Forces every worker to run the project's lint command from its worktree root before any `git commit`. Lint must exit 0 or the commit is blocked. Auto-fixable issues are run through the project's auto-fix step and lint is re-run; remaining real violations halt the worker, who messages the team lead with the violation rather than committing past it. Style consistency is enforced per-slice at the worker boundary instead of being reconstructed at integration time.

## Activation
Per worker, per commit. Injected into every worker dispatch via the worker template. Fires immediately before each `git commit` the worker would otherwise run.

## Modification: references/worker.md:smoke-checks
The following block is injected into the worker template under the "Pre-commit Gate (mandatory)" section:

> Before any `git commit`, run the project's lint command from your worktree root. Exit 0 or do not commit. If lint reports auto-fixable issues, run the project's auto-fix (e.g., `pnpm --filter web exec biome check --write apps/web` for Biome) and re-run lint. If a real lint violation remains, halt and message the team lead with the violation rather than committing past it.
>
> Cross-cutting style consistency cannot be reliably reconstructed at integration time. Each worker handles its own slice. (Source: real-world parallel-worker sprint retro)

## Escape valve
For projects with no lint command, or repos where the project lint step is broken and being repaired, the orchestrator may explicitly drop this block from the worker prompt before dispatch and note the drop in the bracket. There is no runtime per-commit toggle by design — the gate is a worker-side guard.

## Expected delta vs not-having-it
Eliminates a class of cross-worker lint-drift regression observed in real parallel sprints: 12 lint errors accumulated across 8 files from 9 parallel workers, costing ~20 min of post-integration fixup. With every worker complying, the integration-time lint pass is expected to drop to 0 fixups. The cost is shifted to per-worker lint runs (seconds each) instead of a single batched cleanup pass at the end of the sprint.
