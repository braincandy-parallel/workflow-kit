---
mode: default
inherits: none
adds: [pre-commit-lint-gate, pre-flight-readiness-check, paired-value-smoke-checks, out-of-band-instruction-reporting, dev-server-concurrency-contract]
removes: []
status: default
---

# Mode: Default

## Use case
Any sprint without an explicit `mode:` declaration in the PL frontmatter. This is the implicit mode loaded when neither `mode:` nor `features:` is set. It matches the current /implement behavior: five default-on safety features bundled, no experimental work patterns active.

Pick this when:
- The sprint is a single-surface or small-scope build
- Acceptance criteria are still firming up
- Design tokens are not yet locked
- The team has not opted into experimental patterns

## Feature composition
- Inherits from: none
- Adds:
  - `pre-commit-lint-gate` — worker self-lint before commit
  - `pre-flight-readiness-check` — coordinator's 60s pre-dispatch check
  - `paired-value-smoke-checks` — workers grep both expected values
  - `out-of-band-instruction-reporting` — workers report direct user messages
  - `dev-server-concurrency-contract` — one owner per .next/ cache
- Removes: (none)
- Conflicts checked: (none — default mode has no conflict pairs to evaluate)

## Composition warnings
None. All five default-on features are designed to coexist without informational `requires:` notes.

## Expected scorecard delta vs default mode
This is the baseline. No deltas to report.

## When to NOT pick this mode
Default mode is always safe to pick. The only reason to skip it is when a sprint pattern explicitly benefits from an experimental mode (turbo or swarm) and the prerequisites for those modes are satisfied (locked design tokens, stable acceptance criteria, ≥3 analogous surfaces, etc.). When in doubt, stay on default.
