---
date created: 2026-06-17
tags: [spec, google-ads, paid, parallel, marketing]
category: Spec
status: ready-to-implement
---

# SPC - Google Ads Setup: parallel-projects.net

**Project:** Parallel
**Owner:** Luca
**Budget:** $300/month (~$10/day)
**Ad Groups:** 2 (Laser Shows, Lighting Design) — Projection Mapping excluded
**Goal:** Generate planner inquiries via contact form while organic SEO builds
**Platform:** Google Ads (Search only)

---

## 1. Before You Start

Requirements:
- Google Ads account at ads.google.com — create with parallelprojectsmx@gmail.com
- Access to Squarespace (for conversion tracking tag)
- Credit card for billing

---

## 2. Campaign Setup

1. Click **New Campaign**
2. Goal: **Leads**
3. Type: **Search** — not Performance Max, not Display
4. Name: `Parallel - Search - Los Cabos`

---

## 3. Location & Language Targeting

Target by **search intent, not user location.** Wedding planners searching for Los Cabos vendors are based in the US, Canada, and Mexico City — not in Cabo.

- **Locations:** United States, Canada, Mexico
- **Location options:** Presence or interest (catches anyone searching Los Cabos regardless of where they are)
- **Languages:** English, Spanish

---

## 4. Ad Groups & Keywords

Two ad groups, each linking to its own service page.

Use **phrase match** for all keywords — wrap each in quotes in the interface.

### Ad Group 1 — Laser Shows
**Landing page:** `/services/laser-shows`

```
"laser show Los Cabos"
"laser show Cabo San Lucas"
"laser show Cabo"
"wedding laser show Los Cabos"
"laser production Los Cabos"
```

### Ad Group 2 — Lighting Design
**Landing page:** `/services/lighting-design`

```
"lighting design Los Cabos"
"event lighting Los Cabos"
"wedding lighting Los Cabos"
"lighting designer Cabo San Lucas"
"event lighting Cabo"
```

### Negative Keywords (add at campaign level)

```
DIY
rental
rent
cheap
free
how to
tutorial
software
app
```

---

## 5. Ad Copy

### Ad Group 1 — Laser Shows

**Headlines:**
```
Laser Shows in Los Cabos
KVant & Unity Laser Systems
Professional Laser Production
Indoor & Outdoor Laser Shows
Built Around Your Music
Contact Parallel
```

**Descriptions:**
```
Professional-grade KVant and Unity laser systems. Programmed live or to timecode for any event.
We design every show specifically for the event — not a template. Tell us about your project.
```

### Ad Group 2 — Lighting Design

**Headlines:**
```
Event Lighting Design in Cabo
We Work From Your Event
Full Lighting Design & Direction
Ceremony to Reception
Not Just Set Up — Designed
Contact Parallel
```

**Descriptions:**
```
Parallel designs the complete lighting plan for your event, working with any gear. Not just ours.
We've designed lighting at Acre, Viceroy, Chileno Bay, Amara, and more across Los Cabos.
```

---

## 6. Conversion Tracking

Set this up before launching. Without it you have no data on what's working.

1. In Google Ads: **Tools > Conversions > New Conversion Action**
2. Select: **Website**
3. Category: **Submit lead form**
4. Name: `Contact Form Submission`
5. Copy the tag snippet Google provides
6. In Squarespace: **Settings > Advanced > Code Injection > Footer** — paste the tag there
7. Set the conversion trigger to the contact form thank-you URL (Squarespace shows a confirmation page after submission — use that URL)

---

## 7. Budget & Bidding

| Setting | Value |
|---------|-------|
| Daily budget | $10/day |
| Bidding strategy | Maximize Clicks |
| Max CPC cap | $3.00 |
| Ad schedule | All hours (narrow after 4 weeks of data) |

Switch bidding to **Maximize Conversions** once you have 20+ conversions tracked.

---

## 8. Review Schedule

**At 2 weeks:**
- Which keywords are getting impressions vs. clicks
- Any irrelevant search terms appearing (add to negatives)
- Which ad group is spending the most

**At 4 weeks:**
- Cost per inquiry (from conversion tracking)
- Pause any keyword with 50+ clicks and zero conversions
- Shift budget toward the ad group that's converting

---

## 9. Launch Checklist

- [ ] Create Google Ads account at ads.google.com
- [ ] New Search campaign — goal: Leads
- [ ] Location: US, Canada, Mexico — set to Presence or interest
- [ ] Create 2 ad groups with phrase-match keywords (Laser Shows, Lighting Design)
- [ ] Write and enter ads, linking each to its service page
- [ ] Add negative keywords at campaign level
- [ ] Set up conversion tracking tag in Squarespace footer
- [ ] Set daily budget $10, max CPC $3, bidding: Maximize Clicks
- [ ] Launch
- [ ] Calendar reminder: review at 2 weeks and 4 weeks
