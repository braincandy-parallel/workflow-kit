---
date created: 2026-06-16
tags: [pjl, parallel]
category: Project Log
---

# PJL - Parallel

Agent-readable implementation record. Newest entries on top.

---

## 2026-06-17

### SEO Launch + Marketing Specs

**What was done:**
- Completed full SEO implementation session: tagline ("Shaping Space Through Light"), all open questions resolved, Phase 1 and Phase 2 Squarespace work done by Luca
- Google Business Profile created. WhatsApp Business created.
- Updated [[SPC - Website SEO Copy]]: render-to-reality homepage section, standardized CTAs to "Tell us about your project," H1 location references removed, Southern Baja geo scope added
- Created [[SPC - Google Ads Setup]]: Search campaign, 3 ad groups, $300/month, targets planners in US/Canada/Mexico by keyword intent
- Created [[SPC - Instagram Organic Content]]: archive workflow, Reels format, caption formula, tagging strategy -- owner Alonso
- Created [[SPC - Cold Email Outreach]]: 3-email sequence with templates, tracking spreadsheet, reply playbook -- owner Alonso. Validated channel (~$20K USD prior)

**El Ganzo:**
- Alonso accepted 65/35 salary split for this project; proposed 60/40 reevaluation post-project
- 60/40 USD scenario analysis added to [[RE - Hotel el Ganzo Partnership & Budget Overview]]
- Billable hours logged: 2h each to date
- Next gate: on-site visit for photos before creative PDF and renders can progress

**Decisions:**
- SEO Phase 1 and Phase 2 are complete and live on parallel-projects.net
- Google Ads to launch in parallel with SEO (don't wait for organic rankings)
- Cold email and Instagram assigned to Alonso as his projects

**What's next:**
- Launch Google Ads campaign per [[SPC - Google Ads Setup]]
- Alonso to begin Instagram archive workflow per [[SPC - Instagram Organic Content]]
- Alonso to begin cold email outreach per [[SPC - Cold Email Outreach]]
- El Ganzo: schedule site visit to unblock creative PDF and renders

---

### Parallel/Private Split Teardown

**What was done:**
- Decided to stop using the shared GitHub repo approach (`braincandy-parallel/parallel.git`) for Alonso collaboration. Switching to Google Drive instead.
- Removed the `parallel` git remote from vault: `git remote remove parallel`. Only `origin` (`braincandy-parallel/workflow-kit.git`) remains.
- Deleted `02_Projects/parallel-private/` directory. It contained only empty subdirectories (`hotel-el-ganzo/reports/`, `hotel-el-ganzo/specs/`) — never committed, no content to migrate.
- Removed the "Shared vs Private Split" section from `02_Projects/parallel/agents.md`, including all routing rules that gated content between `parallel/` and `parallel-private/`.

**Decisions:**
- Google Drive will replace the GitHub shared repo as the mechanism for sharing parallel project docs with Alonso. No technical setup was done in this session — that's a future step.
- `parallel/` is now the single source of truth for all parallel content. No private/public split exists anymore.
- `parallel-solo/` is unaffected and remains separate.

**What's next:**
- Set up Google Drive folder for Alonso collaboration when ready.
- Potentially archive or delete the `braincandy-parallel/parallel.git` repo on GitHub (not done — confirm with Luca before touching the remote).

---

## 2026-06-16

### SEO Project Kickoff

**What was done:**
- Audited parallel-projects.net: 4 pages, no meta descriptions, no service pages, thin content, Gmail contact. Full findings in [[RE - SEO Audit parallel-projects.net]].
- Researched competitors: Trilogy Production (full-service, weak SEO), DJCO (DJ-focused, not a threat), Instagram-only players (no web presence). Zero direct competitors ranking for Parallel's core services.
- Identified primary keyword opportunities: "projection mapping Los Cabos," "laser show Cabo San Lucas," "lighting design Cabo San Lucas" — all uncontested.
- Created full copy spec [[SPC - Website SEO Copy]]: title tags, meta descriptions, homepage copy, 6 service page briefs with complete body copy, LocalBusiness JSON-LD schema, 10-step implementation checklist.
- Platform confirmed: Squarespace. Primary audience: wedding planners / coordinators. Language: English only.
- Venues confirmed for copy: Acre, One&Only Palmilla, Viceroy, Chileno Bay, Amara, Hotel San Cristobal, Paradero, Kimpton Mas Olas.

**What's next:**
- Tagline selection (3 options in spec — reviewing tomorrow)
- Phase 1 implementation in Squarespace (title tags, meta, homepage copy, schema — ~1hr of work)
- Phase 2: create 6 service landing pages

**Open questions:**
- Branded email to replace Gmail
- Google Business Profile status (exists or needs creation)
- Service page structure: Squarespace pages or Services section with subpages?
- Geographic scope: Los Cabos Corridor only, or also San José / East Cape?

**Artifacts:**
- [[RE - SEO Audit parallel-projects.net]] — `02_Projects/parallel/reports/2026-06-16/`
- [[SPC - Website SEO Copy]] — `02_Projects/parallel/specs/2026-06-16/`
