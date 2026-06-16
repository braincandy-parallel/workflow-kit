---
date created: 2026-06-15
tags: [review, parallel-solo, outside-lands]
category: Review
---
# ARE - Outside Lands Bison Installation Context Brief

## Applicable Lessons

No lessons files exist yet at either the project level (`02_Projects/parallel-solo/lessons.md`) or the root vault level (`lessons.md`). The project `lessons.md` file exists as a stub but contains no recorded lessons. This is the first active project in this vault.

Root `agents.md` confirms parallel-solo is an active project for Luca's independent technical production work, separate from the Parallel cofounder business. No prior lessons from related interactive art or festival projects are on record.

## Related Specs & Plans

This is the only spec in the parallel-solo project. No prior SPC, PL, or WL files exist under `02_Projects/parallel-solo/`. The Bison Installation spec is the first document in this project.

## Architectural Constraints

The following decisions are locked per Section 5 of the spec and cannot be revisited in planning without explicit client agreement:

**Hardware (locked):**
- Three-node ESP32 architecture: one sequencer, one WLED lighting controller, one audio controller
- SK6812 RGBWW 5V LED strip from Parallel internal stock
- Aluminum LED channels from Parallel internal stock
- Audio chain: ESP32 + SD card module + 3.5mm/RCA output to 2x PA monitors (subwoofer pending budget)
- Touch sensor: capacitive type, sensor ordered 2026-06-15, exact model TBD pending physical prototyping

**Communication protocol (locked):**
- ESP32 inter-node communication: UDP over WiFi on a shared LAN
- WLED receives UDP realtime packets from the sequencer ESP32 (not DMX, which is explicitly out of scope)
- Dedicated router/hotspot inside the tech module -- no reliance on festival WiFi

**Physical structure (locked):**
- 12 crated components (4 legs, head, horns, body in 4 pieces, steel base)
- Touch zones: fiberglass/resin snout and horns
- Bison body: 3/4 pine ply
- Base: square tube metal frame with ground anchor points
- Electronics in a single weathersealed, tool-access-only enclosure removable as a unit

**Ownership and scope boundary (locked):**
- Work for hire; Superfly owns all deliverables post-acceptance
- Physical fabrication is Lucille's scope; floral/foliage is creative team's scope
- Audio content is JB (Silent G Studios); on-site logistics are out of scope
- Spec covers 2026 only; 2027+ maintenance is a separate future engagement

**Open decisions still in flight (not locked):**
- WiFi band (2.4GHz vs 5GHz vs ESP-NOW fallback) -- to be determined during prototyping and dry-run
- Exact touch sensor model -- physical prototyping gate before July 1
- Lighting sequence design -- warm/amber/white burst, final in WLED during build
- Spare parts onsite kit -- pending client confirmation

## Lessons That SHOULD Be Captured Post-Project

These are gaps where a future agent or session starting a similar interactive art or festival project would benefit from recorded lessons. Flag each for capture during closeout:

1. **Capacitive touch in fiberglass/resin at a festival.** Sensitivity tuning, debounce values, false trigger rates from crowd/RF/EMI. What sensor type and settings worked.

2. **ESP32 UDP reliability in crowded festival RF environments.** Whether 2.4GHz held up, whether 5GHz was required, whether ESP-NOW was needed as fallback. Actual observed latency vs the 250ms NFR.

3. **WLED UDP trigger configuration.** The exact preset and realtime packet format that reliably fired from the sequencer ESP32 and what pitfalls were encountered.

4. **Audio ESP32 SD card playback stability over 11-hour days.** Whether WAV or MP3 was used, buffer size, any dropout issues, and how volume was safely maxed for festival SPL requirements.

5. **Non-technical crew assembly guide quality.** SAT-9 results -- how long assembly took, what confused the crew member, what photo or label changes improved clarity.

6. **Tech module thermal performance.** Fan sizing, whether the 4-hour dry-run SAT-7 revealed any hotspots, actual component temperatures.

7. **Shipping 12 crated electronics components internationally (Los Cabos to SF).** Customs, crating, what got damaged, what the spare kit saved.

8. **WiFi hotspot inside a sealed metal-adjacent enclosure.** Signal penetration to ESP32 nodes through the Bison body; antenna placement decisions.

9. **JB audio delivery timeline vs production timeline.** Whether the July 10 deadline held, and what placeholder audio strategy is recommended for future builds.

10. **Superfly client dry-run video as acceptance gate.** Whether video-based remote acceptance was sufficient or whether in-person review was required at any point.

## Key References for Reviewer

No REF documents relevant to ESP32, WLED, touch sensing, interactive installations, or festival logistics exist in `04_Reference/`. The only REF files in the vault are `REF - Updating the Workflow Kit.md` and `REF - Onboarding Preferences.md`, which are vault-management docs unrelated to this project.

The reviewer should treat the spec itself as the sole source of architectural and technical context. The following external documentation is implicitly relied upon by the spec but not yet linked in the vault:

- WLED documentation (UDP realtime control API, preset configuration)
- ESP32 capacitive touch GPIO documentation
- SK6812 RGBWW LED strip datasheet (IP65 rating verification)
- DFPlayer Mini or equivalent audio module documentation (if used for audio ESP32)
- IP65 enclosure selection criteria for the tech module

Recommend creating REF docs for WLED UDP trigger setup and ESP32 capacitive touch patterns after prototyping, to support any 2027+ maintenance engagement.
