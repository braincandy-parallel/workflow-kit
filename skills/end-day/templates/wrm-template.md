# WRM Template (Weekly Roadmap)

Replaces the old WF (Weekly Focus) and SOW (Start of Week). The WRM narrows the MRM's monthly scope into a bounded weekly frame. Orient loads it alongside the SOD and MRM.

Agent-facing. Every sentence assumes zero prior context. Ground all nouns. Wikilink all artifacts.

## When produced

Friday (or last workday before weekend), immediately after the EOW. The EOW's pattern analysis and the current MRM are the primary inputs.

## Context to gather

- Current MRM (read its objectives and decision rules)
- This week's EOW (just produced, read its retro findings and goal progress)
- Open PICs (glob `02_Projects/**/PIC - *.md`, status: open or picked-up)
- Previous WRM (for continuity: which goals carried, which completed)

## Step 1: Draft weekly goals

Select exactly 3 goals from the MRM's monthly objectives. Each must have:
- A title tied to the parent MRM objective
- "What done looks like" criteria (concrete, verifiable)
- The PICs that advance this goal

If more than 3 goals compete, the MRM decision rules arbitrate.

## Step 2: Confirm via AskUserQuestion

Present the 3 goals with their done criteria. User confirms or adjusts.

## Template

```markdown
---
date created: {today}
tags: [report, wrm, roadmap, operative]
category: Report
type: WRM
period: {YYYY}-W{ww}
mtd_window_start: {YYYY-MM of current MRM}
parent_mrm: "MRM - {YYYY-MM}"
---

# WRM - Week {ww} ({date range})

## Month-to-date context
{2-3 sentences. Progress against MRM objectives since the month started.
Which objectives advanced, which stalled. Grounded in specific artifacts shipped.}

## Weekly goals
{Exactly 3. Each inherits from an MRM objective.}

### Goal 1: {Title} (MRM: {parent objective})
**Done =** {concrete verifiable criteria}
**PICs:** [[PIC - A]], [[PIC - B]]

### Goal 2: {Title} (MRM: {parent objective})
**Done =** {criteria}
**PICs:** [[PIC - C]]

### Goal 3: {Title} (MRM: {parent objective})
**Done =** {criteria}
**PICs:** [[PIC - D]], [[PIC - E]]

## In scope this week
- {Bulleted list of specific work items, PICs, or decisions that belong this week}

## Out of scope this week
- {Bulleted list of work explicitly deferred. Not "everything else," only items
  that might plausibly pull attention away from the 3 goals.}

## Directives from EOW retro
{Landing zone items routed from the EOW's insight extraction.
Each entry: what behavioral or structural change applies this week.
Omit section if the EOW had no items landing in "next WRM directive."}
- {Directive with specific trigger and action}

## Monday frog
{The single hardest/most important task to start the week with.
One task, stated as a directive.}
```

Save to `01_Notes/Reports/WRM/WRM - {YYYY}-W{ww}.md`.

Keep the WRM **under 400 words**.

## Writing rules

- **Strict inheritance.** Every weekly goal must trace to an MRM monthly objective. No orphan goals.
- **3 goals, not 5.** Matches observed capacity (deep work on 2-3 fronts per week max).
- **In/out lists are the strategic side-trip defense.** The "out of scope" list names specific temptations, not "everything else."
- **Done criteria are verifiable.** "Deployed and tested at YOUR_DOMAIN/kb" not "made progress on KB."
- **Movement, not status.** Frame done criteria around shipped outputs, not time spent.
