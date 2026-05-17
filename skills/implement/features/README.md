# Implement Skill — Features

Atomic behaviors that the /implement skill loads based on PL frontmatter `mode:` + `features:`. Each feature has its own file documenting what it modifies, when it activates, and how to escape if it goes wrong.

## Default-on (loaded by `mode: default`)
- [pre-commit-lint-gate](pre-commit-lint-gate.md) — worker self-lint before commit
- [pre-flight-readiness-check](pre-flight-readiness-check.md) — coordinator's 60s pre-dispatch check
- [paired-value-smoke-checks](paired-value-smoke-checks.md) — workers grep both expected values
- [out-of-band-instruction-reporting](out-of-band-instruction-reporting.md) — workers report direct user messages
- [dev-server-concurrency-contract](dev-server-concurrency-contract.md) — one owner per .next/ cache

## Experimental (loaded by turbo / swarm modes)
- [persistent-dev-server-worker](persistent-dev-server-worker.md) — dedicated agent owns dev server
- [design-pass-on-first-page](design-pass-on-first-page.md) — /frontend-design baked into initial render
- [parallel-harness-author](parallel-harness-author.md) — acceptance runner authored in parallel
- [multi-surface-parallelism](multi-surface-parallelism.md) — derivative surfaces built in parallel
- [two-team-topology](two-team-topology.md) — sub-coordinator team for spec/design alongside impl
