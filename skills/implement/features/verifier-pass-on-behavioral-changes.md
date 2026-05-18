---
feature: verifier-pass-on-behavioral-changes
status: default-on
modifies:
  - SKILL.md:quality
  - references/worker.md:reporting
requires: []
conflicts: []
default_params:
  worker_count_threshold: 3
  io_pairs_per_surface: 3
  prefer_model_diversity: true
source_retro: "2026-05-17 evening - Phase 3 PL execution; IR - Implement Defaults to Sequential When Parallel Was Available, Sub-incident 4"
---

# Verifier Pass on Behavioral Changes

## What it does

Restores the Worker/Verifier role split into `/implement` for behavioral changes. The Worker that wrote the change does NOT verify it. A separate Verifier agent runs pre-selected I/O pairs against the deployed change and reports PASS/FAIL per test with mechanism evidence.

This is the focused adoption of `/qa-coord`'s actor/judge separation principle ([[SD - Pipeline QA Coordination]]) into general implementation work. The full three-role coordination, multi-dimensional matrix (TP/TN/false-positive/false-negative/ambiguity/closed-loop), and pipeline-quality semantics live in `/qa-coord`. This feature is the strict subset that applies to non-pipeline behavioral changes: 1-3 pre-selected I/O pairs per surface, no full matrix.

## Activation

The feature fires when ANY of the following is true:

1. **Standard tier + behavioral acceptance criterion.** Acceptance is phrased "X should happen when Y" or "X should be preserved across Z" or otherwise tests conditional/semantic behavior rather than shape-only.
2. **`worker_count >= 3` in any tier**, including light tier. Multi-surface parallel dispatch needs role-split verification regardless of tier. This is the IR-1 Sub-incident 4 fix: yesterday 7 surface workers ran on light tier without verifiers; the user had to prompt "did you use verifiers for the workers?" The feature now triggers automatically at the 3-worker threshold.
3. **PL frontmatter opt-in** via `features.add: [verifier-pass-on-behavioral-changes]` regardless of tier or worker count.

Threshold is configurable via `default_params.worker_count_threshold` per PL.

## When the Verifier fires (per surface)

After the Worker commits and the Coordinator deploys (smoke checks pass), dispatch a Verifier for each surface with one of:

- Auth/middleware changes (does it reject invalid AND accept valid?)
- Schema migrations with data semantics (does existing data read correctly through new shape?)
- API behavior changes (correct values across conditional paths?)
- Bulk operations with selection logic (right rows touched?)
- Multi-surface UI behavioral changes (each surface's affordance works as specified, not just renders)
- Any acceptance phrased as conditional/semantic behavior

## When to skip the Verifier (smoke checks suffice)

- Pure config flips (env vars, feature flags) where smoke exercises the flag
- Static endpoints whose acceptance criterion IS the smoke check (shape + status)
- Doc / copy edits with no behavioral component
- Additive UI components with no business logic AND only one worker

When in doubt, dispatch. The cost is one Verifier call; the alternative is shipping unverified behavioral changes (and yesterday's IR-1 Sub-incident 4 documents what happens when this is skipped).

## Modification: SKILL.md:quality

Inject the following block into `SKILL.md` between the existing "Quality: Security Eval" and "Gates" sections:

> ## Quality: Verifier Pass (Behavioral Changes)
>
> Loaded from `features/verifier-pass-on-behavioral-changes.md` when active (default-on under `mode: default`).
>
> For changes whose acceptance criterion is **behavioral** (not just "compiles" or "returns 200"), the Worker that wrote the change does NOT verify it. Dispatch a separate Verifier agent per surface to test pre-selected I/O pairs against the deployed change.
>
> **Constitutional reference:** [[SD - Pipeline QA Coordination]]. This is the actor/judge separation principle adopted into general implementation work. For pipeline/LLM-in-the-loop work where data quality is the deliverable, use `/qa-coord` instead — that skill has the full three-role protocol with closed-loop testing and the 6-dimension matrix.
>
> **Mechanism (per surface):**
>
> 1. Worker commits and reports per `references/worker.md` (no self-verification language; see banned-words list below)
> 2. Coordinator (you) deploys via the project's deploy procedure (e.g., `/vps-deploy`, project-specific deploy command, `pnpm build`, container restart). The Worker does NOT deploy.
> 3. Smoke checks run first. If they fail, fix the deploy before dispatching the Verifier — no point verifying a broken deploy.
> 4. Coordinator pulls 1-3 pre-selected I/O pairs from the bracket's validation plan per surface. Minimum coverage: 1 true positive, 1 true negative, plus 1 boundary or false-positive-risk input where applicable.
> 5. Coordinator dispatches the Verifier with the template in `references/verifier.md`. Prefer model diversity — if Worker was sonnet, Verifier is opus; if Worker was opus, Verifier is sonnet. Single-model fallback is acceptable with documented reason ("single-model — budget").
> 6. Verifier returns the I/O proof report (PASS/FAIL per test, no recommendations).
> 7. Coordinator decides per the PASS / FAIL / ESCALATE / INCONCLUSIVE rules in `references/verifier.md`.
>
> **Parallel dispatch case (worker_count >= 3):** When 3+ surface workers ran in parallel, the Verifier pass is also parallel — one Verifier per surface, each reading its surface's SPC + the integration worktree, dispatched in a single tool-call message. Verifier model is chosen per surface independently (opus implementer → sonnet verifier, etc.). Total verifier overhead at this scale is ~10-15 min for the parallel batch.
>
> **Coordinator scope control.** The Verifier reports findings; the Coordinator decides what to do with them. The Verifier cannot:
> - Suggest fixes (it reports only what it observed)
> - Expand scope (no "while I'm here" test additions outside the pre-selected matrix)
> - Interpret partial PASS as PASS overall
>
> Adjacent issues the Verifier notices outside the test scope go in the report's `## Observations (outside test scope)` section. The Coordinator decides whether to spawn a PIC, a fix Worker, or note-and-continue.
>
> **PJL logging on PASS.** After each Verifier PASS, log immediately to the project's PJL (do not defer to closeout). Format: `- **Verifier PASS: {surface/feature}** — {N} tests passed. Commit {hash}. Verifier: {model}. Tests: {input1 → PASS, input2 → PASS}.` If context compacts mid-session, the PJL preserves what was verified.
>
> Source: IR-1 Sub-incident 4 (yesterday's 7-surface parallel build dispatched without verifiers; user had to prompt for them after the fact). [[SD - Pipeline QA Coordination]] (constitutional source for actor/judge separation).

## Modification: references/worker.md:reporting

Inject the following block into `references/worker.md` under the "When Done -- Fix Verification Contract" section, between item 4 and the "Report: ..." line:

> 5. **Banned self-assessment language.** When the active feature set includes `verifier-pass-on-behavioral-changes`, do NOT use the words **"validated", "verified", "working", "complete", "confirmed", "tested and passes"** in your report. These are Verifier-only verdicts. Report only what you CHANGED and where:
>
>    > "Modified `<file>:<line>` to do `<X>`. Commit `<hash>`. Awaiting Verifier on `<surface>`."
>
>    The Coordinator deploys after your report. A separate Verifier with model diversity runs the pre-selected I/O pairs and produces the PASS/FAIL verdict. Your self-assessment is not the verdict; the Verifier's I/O proof is.

This injection is appended, not replacing, the existing fix verification contract — the existing items 1-4 (deployed/running, code path exercised, synthetic injection, sibling sweep) still apply.

## Escape valve

The feature can be disabled per PL via `features.remove: [verifier-pass-on-behavioral-changes]` when:

- The change is purely additive (new component, new endpoint with no conditional logic, new column with no semantics)
- The acceptance criterion is fully captured by a smoke check
- The sprint is a single-worker doc edit or refactor with no behavioral surface

When disabled, document the reason in the bracket's "Validation plan" section so the skip is traceable. Skipping without documentation defaults the feature back on at the next sprint.

The skip threshold `worker_count_threshold` can also be raised in PL frontmatter via `features.config.verifier-pass-on-behavioral-changes.worker_count_threshold` to e.g. 5 if a particular PL has many small workers that don't need verification. Keep the default at 3 for the general case.

## Expected delta vs not-having-it

Targets the IR-1 Sub-incident 4 failure mode (parallel multi-surface dispatch without verifier role split) and the broader pattern of premature completion via Worker-self-validates-own-work.

- **Single-surface standard-tier change:** +1 Verifier dispatch (~3-5 min wall clock) per behavioral phase. Catches mechanism mismatches that smoke checks miss (e.g., output happens to match but via the old code path).
- **Multi-surface parallel build (worker_count >= 3):** +N parallel Verifier dispatches (~10-15 min total wall clock for the batch, since dispatched in parallel). At yesterday's 7-surface scale this would have caught the sleep-seed defect + the pharmacology DISPATCH_TABLE gap + the food MacroSummary FillBar vs BulletBar mismatch in the same dispatch round, instead of after a separate user-prompted verifier pass.
- **Composes with `multi-surface-parallelism`:** the multi-surface feature dispatches N parallel Workers; this feature dispatches N parallel Verifiers in a matched dispatch round. Order: workers → coordinator deploys → verifiers → coordinator decides per surface.

Expected savings: 1 fix cycle per behavioral phase shipped with the role split (vs the 12-finding cleanup sprint yesterday after the no-verifier dispatch, where each finding was a separate fix worker).
