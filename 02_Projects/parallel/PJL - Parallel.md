---
date created: 2026-06-16
tags: [pjl, parallel]
category: Project Log
---

# PJL - Parallel

Agent-readable implementation record. Newest entries on top.

---

## 2026-06-18

### Google Ads Campaign Launch

**What was done:**
- Google Ads account created under parallelprojectsmx@gmail.com (transferred from personal account via admin access)
- Smart Campaign published (not Expert Mode Search) -- forced by GBP promo credit path. Credit is spend-match: spend 7000 MXN, get 7000 MXN back
- Campaign name: Parallel - Laser Shows (Los Cabos)
- 1 ad group: Laser Shows only. Projection Mapping excluded per Luca decision (wants to sell lasers and lighting design). [[SPC - Google Ads Setup]] updated to reflect 2 ad groups (Laser Shows, Lighting Design)
- Keywords: 5 phrase-match -- "laser show Los Cabos", "laser show Cabo San Lucas", "laser show Cabo", "wedding laser show Los Cabos", "laser production Los Cabos"
- Location targeting: US, Canada, Mexico (Smart Campaign limitation -- no "Presence or interest" option available)
- Budget: 174 MXN/day (~$10 USD). Chosen to hit 7000 MXN threshold in ~40 days within qualifying window
- Ad copy: 10 headlines, 4 descriptions, 2 long headlines. Angle: bespoke/premium/custom, no brand names, no indoor/outdoor mention per Luca preference
- Asset group strength: Excellent at time of publish
- Images uploaded: 4 square (1200x1200), 4 landscape (1200x628), 2 vertical (960x1200 4:5 ratio), 1 logo (black on transparent PNG 1200x1200)
- Video: uploaded to YouTube via Google Ads automatic hosting
- Extensions added: 5 sitelinks (Laser Shows, Lighting Design, Contact, Parallel Los Cabos, Custom LED, Projection Mapping), callouts (5), structured snippet (Services), call asset, lead form
- Lead form: "qualified" mode, questions: event type (multiple choice), event date, venue, guest count (multiple choice). No budget question -- deliberate, want to pitch value before qualifying on price
- Campaign published Jun 18, 2026. Pending Google review (est. 1-2 business days)

**Image processing done this session (saved to C:\Users\lucas\Pictures\google ad\ and C:\Users\lucas\Downloads\):**
- Logo: black-on-transparent 1200x1200 PNG, white-on-transparent 1200x1200 PNG (from C:\Users\lucas\Downloads\logo parallel.jpeg)
- square full 1200x1200.jpg, square full 2 1200x1200.jpg, square full 3 1200x1200.jpg, square 4 1200x1200.jpg
- landscape full 1-5 at 1200x628 (landscape full 5 at 1200x799 -- user crops in Google)
- vertical full 960x1200.jpg, vertical full 2 960x1200.jpg (center-cropped from 2:3 to 4:5)

**Decisions:**
- Projection Mapping excluded from current campaign (Luca only wants to sell lasers and lighting design right now)
- Smart Campaign accepted as Phase 1 vehicle -- full control will come in Phase 2 with Expert Mode
- 174 MXN/day budget (not 108 or 347) -- balanced spend rate to hit credit threshold in time
- Lead form set to "more qualified" not "more volume" -- premium bespoke service, quality over quantity
- No price sitelink -- pricing is custom/quote-based
- No location extension -- clients don't visit an office
- GA4 already linked to Squarespace. Google Ads to GA4 link deferred until after campaign clears review

**Still needed:**
- Link Google Ads to GA4: Tools > Linked accounts > Google Analytics 4
- Calendar reminders: 2-week review Jul 2, 2026; 4-week review Jul 16, 2026
- Phase 2: Expert Mode Search campaign once 7000 MXN credit activates (~40 days). Use this campaign's performance data to inform Phase 2 copy, images, and keyword bids

**Artifacts:**
- [[SPC - Google Ads Setup]] -- updated (Projection Mapping removed, 2 ad groups)
- [[PIC - Parallel Google Ads Launch]] -- closed Jun 18, 2026

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
