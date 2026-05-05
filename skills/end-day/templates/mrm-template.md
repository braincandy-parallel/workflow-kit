# MRM Template (Monthly Roadmap)

Replaces the old RM (Strategic Roadmap) and SOM (Start of Month). The MRM is the highest-level operative document. Orient loads it at session start to frame all work for the month.

Agent-facing. Every sentence assumes zero prior context. Ground all nouns. Wikilink all artifacts.

## When produced

Last workday of the month, immediately after the EOM. The EOM's systemic diagnosis and carry-forward list are the primary inputs.

## Context to gather

- This month's EOM (just produced, read its systemic issues, carry-forwards, and "Next Month Setup")
- Previous month's MRM (for format consistency and to check which objectives carried vs completed)
- Open PICs (glob `02_Projects/**/PIC - *.md`, status: open or picked-up)
- Any explicit user-stated objectives from recent SODs or daily notes

## Step 1: Draft objectives

Synthesize 3-5 monthly objectives. Each must have:
- A short title (3-5 words)
- A done definition (concrete shippable artifact, not a vague aspiration)
- The driving reason (deadline, dependency, stakeholder ask, strategic goal alignment)

If the month has 5+ candidate objectives, force the cut to 3-5. More than 5 means none are real priorities.

## Step 2: Confirm via AskUserQuestion

Show the draft objective list. Offer the user 3-4 packagings (e.g., "5 objectives", "4 objectives drop X", "3 objectives just A/B/C"). Iterate until approved.

## Step 3: Draft decision rules

3-5 heuristics agents apply when deciding whether new work belongs in the month. These come from two sources:
- EOM systemic issues that need structural prevention
- Capacity constraints inferred from the previous month's goal completion rate

## Template

```markdown
---
date created: {today}
tags: [report, mrm, roadmap, operative]
category: Report
type: MRM
period: {YYYY-MM new month}
anchor_eom: {YYYY-MM previous month}
---

# MRM - {Month name YYYY}

## Strategic frame
{One paragraph (4-8 sentences). The state entering the month, hard deadlines,
what constraint or tradeoff the month optimizes for, risk signals carrying forward.
Forward-looking only. Don't summarize last month (that's the EOM's job).}

## Monthly objectives
{3-5 objectives. Each gets a bold title, done definition, and driving reason.
Every objective must trace to a concrete artifact or state change.}
- **{Title}** -- Done = {shippable artifact or measurable state}. {Driving reason}.

## Decision rules
{3-5 rules agents apply when deciding whether new work belongs in the month.}
- {Rule with specific trigger and action}

## Systemic landing zones
{Structural changes inherited from the EOM's systemic diagnosis.
Each entry: what governing variable changes, where it lands (CLAUDE.md, skill, WP, etc.).
Omit if the EOM had no systemic issues.}

## Carry-forward
{Open PICs and unresolved decisions from the previous EOM.
One line each: PIC name, which objective it serves (or "unaligned").}
- [[PIC - Name]] -- {objective alignment or "unaligned"}
```

Save to `01_Notes/Reports/MRM/MRM - {YYYY-MM}.md`.

Keep the MRM **under 500 words**.

## Writing rules

- **Forward-looking only.** Don't summarize last month. The EOM does that.
- **Done definitions, not aspirations.** "3 KBs deployed at YOUR_DOMAIN/kb" not "advance KB work."
- **Hard deadlines surfaced.** Name dates if they exist.
- **3-5 objectives, not 10.** Force the cut.
- **Every PIC in carry-forward names its objective alignment.** Unaligned PICs are flagged, not hidden.
- **Decision rules are enforceable.** "If X then Y" format. Agents must be able to mechanically apply them.
