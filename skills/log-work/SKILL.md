---
name: log-work
description: >-
  Log work to the daily note AND project log. Use this skill when the user wants
  to record what they've been working on, says "log this", "log work", "add to
  worked on", "update the daily note", or wants to capture implementation progress.
  Always writes to both layers: daily note (human-readable) and PJL (agent-readable).
  Also trigger on "what did I work on" if the user wants to retroactively log work.
---

# Log Work

Log work to **both layers**: the daily note's `## Worked on` section (brief, human-focused) and the project log PJL file (complete, agent-focused). Both are mandatory on every invocation. There is no "simple mode" -- every piece of work gets recorded in both places.

**Arguments:** $ARGUMENTS -- Description of work, path to a plan, or nothing (will scan conversation context).

## Path Resolution

Read `~/.claude/wfk-paths.json` at startup. Use `vault_root` and `paths` to resolve directory references (e.g., `{paths.daily_notes}/DN - YYYY-MM-DD.md`, `{paths.projects}/<project>/PJL - <Name>.md`). If the file doesn't exist, use defaults and warn once.

## Step 1 -- Determine What to Log

If the user provided arguments, use those. Otherwise, scan the current conversation for:
- Files created or modified (specs, reports, plans, code)
- Deployments or pipeline runs
- Research completed
- Decisions made

Ask the user to confirm what should be logged if it's ambiguous.

### Environment Disambiguation (MANDATORY for deployable code)

If the work being logged touched a deployable application (any project that runs in both a local dev environment and a remote/production environment), the log entry MUST disambiguate which environment was affected. Three valid forms:

- **Local-only iteration** -- write `(local, verified at http://localhost:<port>/<path>)` after the bullet. Be explicit that this did NOT update production.
- **Deployed to production** -- write `(deployed via [deploy command], verified at [production URL])` after the bullet. Include the verification URL you actually hit.
- **Both** -- write `(local + deployed via [deploy command])`.

Bare phrases like "fixed the hydration bug", "deployed the viewer", "tested the admin page" are AMBIGUOUS and create false confidence. Future agents reading the daily note will not know whether production was actually updated. Always include the environment label and a verification URL.

**Push =/= Deploy.** If your project uses a Push =/= Deploy model (where `git push` does NOT auto-deploy), then pushing without running the deploy command means the change is LOCAL ONLY and the log must say so. Check your project's CLAUDE.md for the deploy model.

## Step 2 -- Always Both Layers

Every invocation writes to **both**:
1. **Daily note** (DN) -- brief, human-readable, scannable. What you'd say in a 30-second standup.
2. **Project log** (PJL) -- complete, agent-readable. Commits, file paths, decisions, deployment commands, what was tried.

There is no mode selection. Both layers are mandatory. The only optional artifact is a **Work Log** (`WL -` file) for exceptionally heavy days where even the PJL entry would be too dense (10+ tasks, multi-phase sprints). On those days, the PJL links to the WL.

## Step 3 -- Identify the Topic and Project

Match the work to a **project name** (not a plan name, not an activity name):
- "Owner Portal", not "Portal v3 Phase 2"
- "Signal Engine", not "Spec Compliance Phase 3c"
- "Document Generator", not "DocGen Guided Creation"

The project name is the broadest stable label. Plans, phases, and activities are detail that goes in the project log.

Identify the project path under `02_Projects/` if one exists.

## Step 3b -- Group + Merge Check (Before Writing)

The Worked on section uses a **three-level hierarchy**: group (`###`) > project (`####`) > bullets.

### Determine the group

Match the project to its group. Define your groups in CLAUDE.md or use project directory structure to infer them. Example group structure:

| Group (`###`) | Projects (`####`) |
|---|---|
| Apps | Web App, Admin Panel, Portal, Dashboard, Mail |
| Intelligence | Signal Engine, Data Pipeline, AI Gateway |
| Infrastructure | Server, CI/CD, Local Dev, Cron System |
| Operations | ERP, Reports, Manual Processes |
| Tooling | Workflow Kit, Analytics |

**Standalone items** (Video Research, Process Improvements, Pickup Triage, Vault Maintenance, etc.) get a `###` heading directly -- no group wrapper.

### Merge into existing headings

Before creating a new `####` heading, scan ALL existing headings under `## Worked on`:

1. **Match by project:** All work on the same project goes under ONE `####`. Different plan phases, deployment ops, and bug fixes for the same project share one heading.
2. **Common abbreviations:** Map shorthand to canonical project names (define mappings in CLAUDE.md if needed).
3. **If the group `###` heading already exists**, add the new `####` project under it. If the group doesn't exist yet, create both.

**The daily note should have ONE `####` heading per project, not one per plan, phase, activity, or session.**

## Step 4 -- Write Both Layers

Every invocation writes both. Do the PJL first (it's the source of truth), then summarize to the daily note.

### 4a. Project Log (PJL) -- Agent Layer

1. **Find or create the PJL file** at the project root:
   - `02_Projects/<project>/PJL - <Project Name>.md`
   - No project: `01_Notes/Project Logs/PJL - Topic.md`
   - **One PJL per project.** If one already exists, append to it.

2. **If creating new:** use frontmatter:
   ```yaml
   ---
   date created: YYYY-MM-DD
   tags: [project-log, <project-tag>]
   category: Project Log
   project: "<project name>"
   ---
   ```

3. **Append entries** under today's date heading (`## YYYY-MM-DD`):
   - Create the date heading if it doesn't exist yet
   - **Newest date section at the top** (below frontmatter / title)
   - **Light days:** inline bullets under the date heading
   - **Heavy days** (major sprint, 10+ tasks, multi-phase work): write a brief summary in the PJL entry, then create a dedicated **Work Log** (`WL -` file) in `work-logs/` and link it: `[[WL - Topic YYYY-MM-DD|Full session log]]`

4. **Write for agent memory, not humans.** The daily note is the human layer; the PJL is the machine layer. Include:
   - Exact file paths, function names, table/column names, container names
   - Commit hashes and migration filenames
   - What was deployed, where, and how (environment + URL + command used)
   - Decisions made and **why** (so future agents don't re-litigate)
   - Known issues, blockers, and workarounds still in place
   - What was tried and didn't work (prevents repeating failures)
   - Wikilinks to specs, plans, PICs, IRs, ADRs

### Work Log File (WL) -- Heavy Days Only

Only create a WL when the PJL entry would be too dense:
- `02_Projects/<project>/work-logs/WL - <Topic> <YYYY-MM-DD>.md`
- **All granular detail goes here** -- commit hashes, migration numbers, per-task breakdowns, component lists, bug lists
- Linked from the PJL entry for that date

### 4b. Dispatch Fresh DN Distillation Subagent (MANDATORY before writing DN)

The PJL is written. The session agent now holds full context: every commit hash, file path, count, and phase number touched during the work. That context is the contamination that causes DN drift. Asking the same agent to self-compress reliably fails because the technical detail looks "informative" rather than "banned." To prevent drift structurally, the DN entry is authored by a fresh subagent that reads ONLY the PJL and the writing profile, not the conversation.

**Constitutional rationale:** This is the same gap class the Worker/Verifier/Coordinator pattern addresses (see `/qa-coord`). The agent who did the work cannot fairly summarize it for a different audience.

1. **Gather materials.** Collect for the subagent:
   - The full text of today's PJL entry (or entries, if multiple projects were logged this session). Include the date heading and all bullets you just wrote.
   - The contents of any writing profile your vault uses for daily notes (e.g. a `WP - DN.md` reference doc). If none exists, skip this material.
   - The contents of the matching `## Worked on` section in today's DN. If no `#### <Project>` heading exists yet, note "no existing entry for this project".
   - The group + merge table from Step 3b so the subagent knows where bullets belong.

2. **Dispatch one `Agent` call with `subagent_type: general-purpose`.** A single subagent handles all projects logged this session. Do not dispatch per-project. Do not use a team for a single dispatch.

3. **Pass this prompt verbatim, with the materials interpolated:**

   ```
   You are distilling project log (PJL) entries into daily note (DN) bullets.

   Audience: the user scanning their day + AI agents at orient time. The DN
   is a scannable table of contents, NOT a technical log. The PJL holds detail.

   Read ONLY the materials I provide below. Do not run searches, reads, or
   tool calls of your own. You will not have access to conversation context,
   and that is intentional.

   --- PJL ENTRIES WRITTEN THIS SESSION ---
   {paste full PJL entry text per project, with project name labels}

   --- WRITING PROFILE ---
   {paste any daily-note writing profile, or "no profile configured"}

   --- EXISTING DN WORKED-ON SECTION ---
   {paste current ## Worked on section, or "section empty"}

   --- GROUP TABLE ---
   {paste the group + project table from log-work Step 3b}

   Return a single block of markdown bullets organized by group and project,
   ready to drop into the DN. Format:

   ### <Group>
   #### <Project>
   - **<bold one key outcome>** -- <single distinct fact>
   - <second distinct outcome>
   - <third distinct outcome if needed> -- [[PJL - <Project>|Project log]]

   Rules:
   - Most projects need 2-3 bullets. Hard cap at 4 unless splitting into
     sub-project headings (see below).
   - **SELECT and CUT, do not GROUP.** When a PJL has 5+ distinct outcomes,
     pick the 2-3 most important and CUT the rest. Do not bundle multiple
     outcomes into one bullet by chaining with commas and "ands." If you
     find yourself writing "did X, restoring Y, adding Z, and fixing W,"
     stop and pick the single most important outcome for that bullet.
   - **One distinct outcome per bullet.** No "and" linking two ideas. No
     comma-chained outcome lists. If a draft bullet contains two distinct
     outcomes, split into two bullets OR cut the less important one.
     Run-on bullets stacking multiple facts are a violation, even if all
     the facts are individually compliant.
   - **Heavy-day sub-projects.** When a project genuinely has 5+ distinct
     outcome blocks worth tracking (e.g. a sprint touched the same project
     in 3 different ways), split into sub-project `#### Project (sub-area)`
     headings with 2-3 bullets each. The sub-area label comes from the
     PJL's `###` subsection headings if present.
   - Bold exactly one key outcome per project (or per sub-project heading).
   - End the last bullet of each project with the PJL wikilink.
   - If merging with existing DN bullets, return the FULL merged set; the
     main agent will replace the heading's contents wholesale.
   - For deployable code, preserve any environment label (LOCAL / deployed
     to production URL with verification URL) from the PJL.
   - **Use literal ampersand `&` in headings**, never the HTML entity
     `&amp;`.

   BANNED in DN output (these are PJL-only; do not include any of them):
   - Commit hashes (hex strings)
   - Migration filenames
   - File paths (anything starting with ~/, /, ./, or containing /, except
     production verification URLs)
   - Function, class, method names (anything ending in () or CamelCase code)
   - Container, schema, table, column, flag, env, header names
   - Counts, percentages, ratios, totals (e.g. "8 threads", "5/5 PASS",
     "176 tests", "349 -> 278 questions", "62/69 tasks")
   - Phase numbers, task IDs, sub-incident refs, retro change letters
   - Lesson numbers (L followed by digits), skill names with leading /,
     frontmatter field names, memory file names
   - Library, tool, framework names
   - UUIDs, error class names, HTTP header names
   - Test counts, oracle scores, review grades

   If a PJL entry does not contain a clear human-readable outcome, return
   one bullet stating that and link to the PJL. Do not invent outcomes.

   Return ONLY the markdown bullet block. No preamble, no commentary,
   no explanation of choices. The main agent will write your output
   verbatim into the DN.
   ```

4. **Use the returned bullets verbatim in Step 4c.** Do not edit, re-derive, expand, or "improve" them from your own session context. If the subagent's output is clearly wrong (empty, formatting broken, banned tokens leaked), dispatch a second subagent with a corrective note rather than fixing it yourself. The contamination problem is structural, and a hand-edit reintroduces it.

### 4c. Daily Note (DN) -- Human Layer

1. Read today's daily note: `01_Notes/Daily/DN - YYYY-MM-DD.md`
2. **Run the group + merge check (Step 3b)** -- find the group and any existing heading for this project.
3. **Write the bullets returned by the subagent in 4b verbatim.** The subagent already applied the writing profile, group/merge table, banned-token list, and PJL wikilinks. Do not re-derive from your own conversation context. Do not edit, expand, or "improve" the output. If the subagent's output is broken, dispatch a second subagent with a corrective note rather than hand-fixing.
4. If merging with existing bullets under a matching `####` heading, replace the entire bullet list under that heading with the subagent's returned set wholesale (the subagent already produced the merged result).
5. If the subagent created new `### Group` or `#### Project` headings that don't exist yet in the DN, add them. New groups go at the **top** of the Worked on section. Standalone items (no group) use `###` directly.
6. **HARD RULE: 4 lines or fewer per project (heading + 3 bullets max).** Heavy-day projects split into sub-project headings (the subagent does this in 4b). The daily note is a table of contents; everything else is in the PJL (and WL for heavy days).

**Good daily note (heavy day, multiple groups):**
```markdown
## Worked on

### Apps
#### Owner Portal
- **Built the full owner portal** -- authentication, ownership cards, voting flow, and admin controls all live
- Ran live UAT, caught and fixed 9 bugs during testing
- Bulk signup emails and final review still needed -- [[PJL - Owner Portal|Project log]]
#### Document Generator
- **Shipped the guided document creation flow** -- overlay wizard walks users through research and generation -- [[PJL - Document Generator|Project log]]

### Intelligence
#### Signal Engine
- **Verified signal pipeline works after gateway refactor** -- fixed two bugs that were crashing email processing and meeting tracking
- Cleaned up stuck processing runs and backfilled missing staff records -- [[PJL - Signal Engine|Project log]]

### Infrastructure
- **Fixed the stale image deploy bug** and built automatic cron-to-database sync -- [[PJL - Infrastructure|Project log]]

### Video Research
- Watched 4 videos, 2 deep dives on harness design and context engineering for coding agents
```

**BAD daily note (too technical):**
```markdown
#### Signal Engine
- **Pipeline health check after gateway refactor** -- gateway integration verified working, Phase 3a entity resolution 6/7 PASS, 2,763 stuck runs cleaned up
- Fixed two pipeline bugs: run-tracking (dirty transaction retry) and classification query crash (`attachment_names` column)
- Disabled all crons until validation complete; backfilled people tiers (jamie to manager, system@ as system)
```
Phase numbers, pass/fail counts, row counts, column names, function names -- all PJL material. The daily note should tell you what moved the project forward, not how the sausage was made.

**Also BAD (build-log style):**
```markdown
#### Owner Portal
- **Phases 0-6 complete** (62/69 tasks) -- auth, ownership, voting, admin
- 10 schema migrations, 9 bugs fixed
- Phase 7 cutover remaining
```
Phase numbers, task counts, and migration counts are for the PJL.

## Formatting Rules

Load and follow any writing profiles configured for daily notes in your vault. Workflow-specific rules that stay here:

- **Three-level hierarchy:** `## Worked on` > `### Group` > `#### Project`
- **ONE `####` heading per project**, never per plan, phase, or activity
- **4 lines or fewer per project** (heading + 3 bullets max)
- Bold the key accomplishment or outcome
- Wikilink output artifacts and the PJL file
- **Nothing technical in the DN.** No commit hashes, migration filenames, component lists, file counts, phase numbers, pass/fail counts, row counts, column names, function names, container names, or validation metrics
- No prose paragraphs, bullets only
- Newer groups at top of `## Worked on` section
- Standalone items (no group) use `###` directly
- NO "Lifelogs & Meeting Notes" subsection, those go in Meetings

## Examples

See `references/examples.md` for good/bad daily note and work log formatting examples.

## Local Customizations

If `LOCAL.md` exists in this skill directory, load and follow it after these instructions. Local instructions override upstream where they conflict.
