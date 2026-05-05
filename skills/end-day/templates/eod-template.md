# EOD Template

## Writing Rules (apply to every section)

1. **Movement, not status.** Only report things that moved. "Deployed 4 fixes" yes. "Worked on signal engine" no. If a project got time but produced no shippable outcome, skip it unless there's a lesson.
2. **Conclusion first.** Lead every bullet with the outcome. "Portal blocked: pipeline captured 1/3 of knowledge" not "We ran 3 depth passes and discovered..."
3. **Metrics first.** Lead with numbers or concrete artifacts. "Deployed 4 fixes. Wrote 202-line doc. Filed 1 IR." Not "Made progress on signal quality and KB structure."
4. **Assume zero context.** Every noun grounded. Not "the pipeline" but "the email corpus pipeline (the system that reads emails and generates knowledge base articles)." A reader on Monday morning should understand every bullet without reading the daily note.
5. **No jargon without parenthetical.** Acronyms and project-specific terms need a brief explanation on first use within the EOD. The reader may not remember what these stand for after a weekend.
6. **Always wikilink artifacts.** Every mention of a filed, created, or updated artifact must include its wikilink. "Filed a high-severity IR" is not actionable. "Filed [[IR - Pipeline Phase Skipping]]" is. The link IS the value.

## Content Quality Tests (apply to every retro item and every "What Shipped" bullet)

Every item must pass ALL 4 tests or it gets cut:
1. **Single Data Point:** anchored to one specific fact or number, not a vague trend
2. **If-This-Then-That:** would a changed number prompt a different decision?
3. **Ownership:** has a named owner, artifact, or completed action
4. **Decision Test:** if removed, would any decision change?

**Filtering heuristic:** Keep if it shipped, changed a decision, or is blocked. Cut if it describes activity not outcomes and removing it changes no decision.

```markdown
---
date created: {today}
tags: [report, eod]
category: Report
type: EOD
period: {today}
day_rating: {1-5}
---

# EOD - {today formatted}

## Summary
{2-3 sentences. The executive summary a reader scans in 10 seconds.
Lead with what shipped. Name the single most important outcome.
End with what's blocked or deferred, if anything.
This section alone should tell someone "what happened today" without reading further.}

## Priority Check
{Compare today's work against the SOD priorities.}
- **{Priority name}**: hit / partial / missed. {One sentence with the concrete evidence. Include a number, artifact name, or deployment URL.}

## What Shipped
{Bullets of concrete deliverables. Each bullet starts with a number or artifact name.
Group by project. Include environment (local/deployed) for code work.
Skip projects that got time but didn't produce a shippable outcome.}

### {Project Name}
- {number/artifact}: {what it does, why it matters in one sentence}

## Retro
{2-4 bullets ONLY. Each must have all four parts or it gets cut.
Scope: tactical observations (bug patterns, process violations, blocked paths, time sinks).}
- **Observation:** {what specifically happened, grounded in a project name and system}
  **Impact:** {what would be different if this hadn't happened}
  **Action:** {what changed because of it, or what should change}
  **Landing zone:** {where this insight produces structural change: next SOD flag | CLAUDE.md rule | L-entry | feedback memory | **[Single-Loop]** if no structural change}

{The 3-day rule: before writing retro items, check if any deferred item has appeared
in 3+ consecutive EODs with only "carry forward" as the action. If so, flag it:
"[ESCALATE TO EOW] -- 3+ day deferral pattern, needs governing-variable change."}

{Examples of GOOD retro items:}
{- **Observation:** 5 fabricated names in KB campaigns. Claude constructed full names from nearby context.}
{  **Impact:** Staff would receive campaigns referencing non-existent colleagues.}
{  **Action:** Built named entity verification into WP-General, added Rule 3, gated skills.}
{  **Landing zone:** CLAUDE.md rule + writing profile + skill gates}

{Examples of SINGLE-LOOP items (flag, don't cut):}
{- **Observation:** Process Entity deferred again (day 5).}
{  **Impact:** SATs remain untested.}
{  **Action:** Tomorrow's frog with hard time-box.}
{  **Landing zone:** [Single-Loop] -- next SOD flag only. [ESCALATE TO EOW]}

## Goals
{Confirmed goals from user. Carry forward from previous EODs until completed or dropped.
Use multiSelect when confirming with user.}
- [x] **{Goal}** -- {one-line status update with concrete evidence}

## Carry Forward
{PICs created or updated today. One line each: PIC name, project, the single next step.
If none: "All work completed or already tracked."}

## Tomorrow's Frog
{The single hardest/most important task to start with tomorrow.
Not a list. One task, stated as a directive.
"Start with: Schema migration for the owner portal. Demo is Tuesday."}

## System Changes
{Skills, CLAUDE.md, templates, or workflow infrastructure modified today.
Only include if something changed. Each entry: what changed, why, what it affects.
Omit entirely if no meta-work today.}
```

Save to `01_Notes/Reports/EOD/EOD - {today}.md`.
