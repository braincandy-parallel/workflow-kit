---
category: Review
date created: 2026-06-15
source: "review-spec skill — Critical Reviewer"
spec: "[[SPC - Outside Lands Bison Installation]]"
---

# Spec Review: Outside Lands Bison Installation

## Spec Quality Score

| Criteria | Rating | Notes |
|----------|--------|-------|
| Completeness | 2 | Strong core requirements; notable gaps in retrigger timing spec, crating plan, head/sensor assembly sequence, and Lucille coordination deadlines |
| Feasibility | 2 | Timeline is achievable but has zero slack. Two hard dependencies (JB audio, Lucille body cavity dims) have no named fallback paths. Any slip on either collapses the dry-run window |
| Clarity | 2 | Most requirements are precise. Retrigger lock mechanism has an ambiguity (who detects end-of-sequence?). Fan rain protection spec says "mesh + baffle or similar" without a testable standard |
| Scope Control | 3 | Boundaries are well-drawn. Out-of-scope list is explicit. Work-for-hire ownership is clean. YOY scope is explicitly limited to 2026 |
| **Total** | **9/12** | |

**Gate:** CONDITIONAL

## Summary Verdict

The spec is structurally sound and the architecture is coherent, but it ships with two unresolved hard dependencies (JB audio delivery and Lucille body cavity dimensions) that have no escalation or fallback procedures documented. The retrigger lock mechanism also has a sequencer-to-itself feedback gap that needs one additional sentence to close. These are fixable with targeted additions before planning begins.

## Clarifications Needed

**CL-1:** How does the sequencer ESP32 know the sequence has ended? The spec says the sequencer clears the retrigger lock "at sequence end" (Section 9, data flow step 6), but the sequencer fires UDP and then has no return channel from WLED or the audio ESP32. Does the sequencer clear the lock on a timer (equal to known audio file duration)? If so, what happens when the audio file duration changes (e.g., JB delivers a longer cut)? This matters because the plan will need to either hard-code a timer or implement a return-channel protocol -- two meaningfully different implementations.

**CL-2:** What is the exact fallback if the subwoofer is cut from budget? The spec lists the sub as "SHOULD" (FR-12b) and "pending budget confirmation," but NFR-6 requires clearly audible audio at 3 meters in a loud festival environment. A bison roar with heavy low-frequency content will not meet NFR-6 with monitors only. Is NFR-6 re-evaluated without the sub, or does the sub become a hard requirement if audio content is low-frequency-heavy? This must be decided before the dry-run so the test uses final hardware.

**CL-3:** What is the hard deadline for Lucille to provide interior body cavity dimensions and mounting plan to Luca? The spec notes this dependency in A-3 and the Scope Analysis flags it as a gap, but no date appears anywhere. Luca cannot size the tech module enclosure without this. If Lucille is late to provide dims, what is Luca's contingency (build for a nominal size and pad, buy a standard NEMA enclosure range)?

**CL-4:** Who performs SAT-9, when, and where? The spec requires "a person unfamiliar with the installation" to assemble the tech module using only the guide. Who is this person -- a friend, a Superfly crew member, a studio assistant? When is this dry run scheduled (it must be complete before the pre-ship video is filmed, which must happen before July 21)? The timeline is tight enough that this needs a named date.

**CL-5:** Is JB physically onsite at Outside Lands, or is "onsite technical contact" (A-7) a remote support role? If JB is the audio content provider but not an electronics technician, naming JB as the hardware troubleshooting contact may create a false safety net. What is the actual scope of JB's onsite role?

## Issues

**C1 | Retrigger lock end-condition is undefined | Section 3.2, FR-4 / Section 9 data flow**
The spec says the sequencer MUST ignore touch input while a sequence is playing, and MUST clear the lock at sequence end. But the sequencer broadcasts UDP and does not receive any return signal. There is no defined mechanism for the sequencer to know when the sequence has ended. The plan will invent this mechanism (most likely a hard-coded timer = audio file duration), which introduces a silent fragility: if audio duration changes, the retrigger lock window silently miscalibrates. Suggestion: add one sentence to FR-4 specifying the lock-clear mechanism -- either "sequencer clears lock after N seconds (equal to audio file duration, configured as a constant)" or "sequencer receives an end-of-sequence UDP packet from the audio ESP32." Pick one and document it.

**C2 | Lucille body cavity deadline is not specified | Section 6, A-3 / Scope Analysis external dependencies**
A-3 assumes Lucille's structure will have pre-planned mounting points before Luca integrates the tech module, but no date is given for when Lucille must deliver interior dimensions. Without dimensions, Luca cannot size the enclosure. The timeline from June 15 to a July 15-19 ready date is 30-34 days. If Luca needs even 2 weeks to build and test the module after receiving dims, Lucille must deliver by July 1. That deadline does not appear in the spec. Suggestion: add a dated milestone to Section 6: "Lucille provides interior body cavity dimensions and cable routing plan to Luca by [DATE]."

**W1 | Fan rain protection spec is not testable | Section 3.5, FR-19**
FR-19 says the fan inlet/outlet MUST be protected against rain ingress with "mesh + baffle or similar." "Or similar" is not a testable standard. SAT-8 accepts visual inspection as the verification method, which means any baffle that looks plausible passes. In a light-rain SF festival environment, this is insufficient. Suggestion: specify the protection standard more precisely -- either reference an IP rating for the fan/vent assembly, specify the baffle geometry (e.g., downward-facing louvers with minimum 50mm overhang), or require a water pour test (pour 250ml over the vent at 45 degrees, no ingress to enclosure interior).

**W2 | No escalation path if JB audio is late | Section 6, A-1 / Section 10 risks**
The spec correctly identifies late JB delivery as High likelihood / High impact and recommends a placeholder audio file by July 5. But there is no escalation procedure: who contacts JB if files are not received by July 5? What format must the files be in? What happens if JB delivers unusable audio (wrong sample rate, clipped, wrong duration) on July 10 with no time to iterate? Suggestion: add a brief escalation note to A-1: "If files not received by July 5, Luca escalates directly to [JB contact]. If final files not received by July 10, ship with placeholder audio and coordinate post-ship remote SD card update procedure with Superfly crew."

**W3 | Crating plan is entirely absent | Section 7 deliverables**
The spec lists 12 crated components shipping from Los Cabos to San Francisco but says nothing about crating design, padding, orientation markings, or moisture protection. The tech module (3 ESP32s, router, SD card with audio content) and fiberglass horns are fragile. The Scope Analysis flags this as a risk. Crating is not called out as a deliverable. Suggestion: add "Crating specification and packing guide" to Section 7 deliverables, even if brief. At minimum: who is responsible for crating, what fragile components require foam/cushioning, and whether moisture protection is required for the Los Cabos-to-SF transit.

**W4 | JB as onsite technical contact is a single point of failure | Section 6, A-7**
A-7 names JB as the onsite technical contact. JB's domain is audio content production, not ESP32 electronics. If a WiFi node drops, a connector works loose, or the cooling fan fails during the festival, JB may not have the skills to diagnose or fix it. There is also no named backup. Suggestion: either (a) confirm JB has basic electronics troubleshooting capability and document it, or (b) name a backup technical contact, or (c) accept the risk explicitly and document that the installation guide troubleshooting tree is the sole fallback for hardware issues.

**W5 | WiFi hotspot inside a sealed enclosure may degrade ESP32 signal | Section 5, Section 9**
The router/hotspot is housed inside the weathersealed tech module, which sits inside the Bison body (3/4 pine ply). The Bison body is then in a dense festival crowd. WiFi signal from a sealed enclosure through wood in a crowded RF environment is a compounding attenuation problem. The spec notes 5GHz or ESP-NOW as fallbacks but does not require testing the actual signal path (router inside enclosure, ESP32 nodes at their mounted positions) before the dry-run. Suggestion: add an explicit prototype test requirement -- measure RSSI from each ESP32 node with the router inside a sealed enclosure mockup before locking antenna placement.

**I1 | Head attachment sequencing is unresolved | Scope Analysis**
The Scope Analysis notes that it is unclear whether the head attaches to the body before or after touch sensor wires are routed to the sequencer. If the head is a closed fiberglass shell, sensor wires must be pre-routed through the head before attachment -- a step that does not currently appear in any deliverable. This is primarily an installation guide concern, but if the sensor wire routing is not validated with Lucille's head design before the guide is written, the guide may be wrong. Flag for resolution during structure coordination.

**I2 | LED channel mounting timing and access window | Scope Analysis**
LED strips in aluminum channels must be mounted inside Bison body panels. If channels are mounted before the body is closed, they must be pre-mounted by Luca before handoff to Lucille (or coordinated assembly). If mounted after, access may be limited. The spec does not define when and by whom LED channel mounting happens relative to body assembly. This is low risk if caught early in the build phase but becomes high risk if both parties assume the other handles it.

**I3 | No defined audio file format or duration constraint | Section 3.3, FR-11**
FR-11 says audio content MUST be WAV or MP3. It does not specify sample rate, bit depth, channel count (mono vs stereo), or maximum duration. Maximum duration matters because it determines the retrigger lock window (see C1). If JB delivers a 45-second file and Luca has coded a 30-second timer, the lock clears 15 seconds early. Suggestion: add to FR-11 or A-1: "Audio file MUST be stereo, 44.1kHz or 48kHz, 16-bit minimum. Duration MUST be agreed between Luca and JB before firmware timer is configured."

**I4 | SAT-7 four-hour test is shorter than the 11-hour operating day | Section 8, SAT-7**
SAT-7 requires a 4-hour continuous dry-run as the thermal/uptime verification. The NFR-2 requirement is 11 hours per day. A 4-hour test will catch most thermal issues but will miss failures that manifest after 6-8 hours (capacitor heat soak, SD card wear, WiFi router overheating). This is accepted risk, not a spec error, but should be explicitly acknowledged. Suggestion: add a note to SAT-7 that the 4-hour test is a practical proxy for 11-hour operation and that thermal readings at hour 4 should show a stable plateau (not still rising).

## Suggested Spec Improvements

1. **FR-4 -- add retrigger lock clear mechanism.** After "retrigger delay = duration of full sequence," add: "The sequencer clears the retrigger lock via an internal timer set to the audio file duration (configured constant). This constant MUST be updated whenever the audio file changes."

2. **A-1 -- add escalation clause.** After the July 10 deadline, add: "If final audio files are not received by July 5, Luca escalates directly to JB. If not received by July 10, Luca ships with placeholder audio and documents a post-ship remote SD card update procedure for Superfly crew."

3. **A-3 -- add Lucille dimension deadline.** Change to: "The Bison structure from Lucille will have pre-drilled or pre-planned mounting points for LED channels and cable routing before Luca integrates the tech module. Lucille MUST provide interior body cavity dimensions and cable routing plan to Luca by [DATE -- recommend July 1]."

4. **FR-19 -- tighten fan rain protection spec.** Replace "mesh + baffle or similar" with a testable description: "downward-facing louvers or a drip-shield baffle with minimum 40mm overhang, such that water poured at 45 degrees over the vent does not reach the fan blade or enclosure interior."

5. **FR-11 -- add audio file format constraints.** Add: "Audio file MUST be stereo WAV or MP3, 44.1kHz or 48kHz, 16-bit minimum. Final file duration MUST be confirmed to Luca before firmware retrigger timer is configured."

6. **Section 7 -- add crating specification as a deliverable.** Add under Documentation: "Crating specification and packing guide: padding requirements per component, orientation markings, fragile labels, moisture protection for tech module."

7. **SAT-9 -- specify who and when.** Replace "a person unfamiliar with the installation" with: "A named crew member (to be identified by Luca or Superfly) unfamiliar with the installation. Dry run MUST be completed by [DATE -- recommend July 18] to allow guide revisions before the pre-ship video is filmed."

8. **Section 6, A-7 -- clarify JB onsite scope.** Add: "JB's onsite role covers audio content issues (volume levels, playback quality). Hardware troubleshooting (ESP32, WiFi, lighting) is handled via the installation guide troubleshooting tree. If hardware issues exceed the guide's coverage, Luca is the escalation contact [define contact method: phone, WhatsApp, etc.]."

9. **Section 10 risks -- add WiFi signal path through enclosure as explicit risk.** Add a row: "Router inside sealed enclosure through pine ply body | Low-Medium | Medium | Validate ESP32 RSSI at mounted positions with router inside sealed mockup during prototype phase."

## Clarification Log

**CL-1:** How does the sequencer ESP32 know the sequence has ended?
**Answer:** Hard-coded timer equal to audio file duration, configured as a named constant. Constant must be updated whenever the audio file changes. (2026-06-15)

**CL-2:** What is the exact fallback if the subwoofer is cut from budget?
**Answer:** Monitors-only is the accepted fallback. NFR-6 (audibility at 3m) is evaluated against the monitors-only configuration. Sub remains a SHOULD, not a hard requirement. (2026-06-15)

**CL-3:** What is the hard deadline for Lucille to provide interior body cavity dimensions?
**Answer:** Non-issue — BRAIN enclosure is already sized, approved by Lucille, and hard-mounted in the Bison HEAD (not the body). Lucille needs to pre-plan LED channel paths and cable pass-throughs into body panels; head assembly sequence (open vs closed relative to sensor wire routing) to be confirmed during build coordination. (2026-06-15)

**CL-4:** Who performs SAT-9, when, and where?
**Answer:** JB or Ruts (META crew) perform assembly in SF warehouse before load-in. Luca available remotely if guide is unclear. (2026-06-15)

**CL-5:** Is JB physically onsite at Outside Lands, or remote?
**Answer:** JB is physically onsite. JB's onsite role covers audio content (volume, playback quality). Hardware troubleshooting (ESP32, WiFi, lighting) is handled via the installation guide troubleshooting tree; Luca is the remote escalation contact for issues beyond the guide's coverage. (2026-06-15)

## Applied Changes

- [x] FR-4: Added retrigger lock clear mechanism (auto-applied — implements CL-1)
- [x] FR-12b: Added monitors-only fallback and NFR-6 evaluation clause (auto-applied — implements CL-2)
- [x] FR-11: Added audio file format constraints — stereo, 44.1/48kHz, 16-bit minimum; duration confirmation requirement (auto-applied — resolves I3)
- [x] Constraints/Hardware: BRAIN enclosure updated — hard-mounted in Bison HEAD, approved by Lucille, contains all electronics (auto-applied — implements CL-3)
- [x] A-1: Added JB escalation clause — July 5 escalate, July 10 ship with placeholder + SD update procedure (auto-applied — resolves W2)
- [x] A-3: Updated to reflect BRAIN in head, coordinate LED paths and head assembly sequence with Lucille (auto-applied — implements CL-3)
- [x] A-7: Clarified JB onsite scope — audio content only; hardware via guide + Luca remote escalation; Ruts added as named crew (auto-applied — implements CL-5)
- [x] SAT-9: Updated to name META crew (JB/Ruts) as assemblers in SF warehouse (auto-applied — implements CL-4)
- [x] FR-19: Fan rain protection tightened — downward-facing louvers or 180-degree baffle duct, 250ml water pour test (user-approved W1)
- [x] SAT-8: Updated to include water pour test as verification method (user-approved W1)
- [x] Section 7: Added crating specification and packing guide as deliverable (user-approved W3)
- [x] SAT-7: Added proxy note — 4-hour test accepted for 11-hour day; thermal plateau required at hour 4 (user-approved I4)
- [ ] W5: WiFi RSSI prototype test requirement (skipped by user)

## Gotchas & Lessons to Watch

| Concern | Spec Section | Risk |
|---------|-------------|------|
| Capacitive sensing through fiberglass -- signal may be blocked entirely depending on resin thickness and conductor area | FR-1, Scope Analysis | If sensor fails behind fiberglass shell, only options are drill-through sensor or surface mount pad -- either requires Lucille's approval and may change the visual design |
| Retrigger lock timer drift if audio file changes after firmware is written | FR-4 | Silent desync: system appears to work but lock clears early or late; visitors can double-trigger or get stuck in extended lock |
| Router antenna placement inside a sealed wood-and-metal-adjacent enclosure | Section 5, A-4 | Can degrade UDP reliability without any visible failure indicator until a live crowd creates full RF load |
| JB audio delivery is on the critical path with no buffer | A-1, Section 10 | July 10 delivery + load + test + dry-run + video all must complete by July 21. One late day from JB collapses the entire pre-ship window |
| Subwoofer budget gate unresolved at spec time | FR-12b, Section 11 | Dry-run tests wrong hardware configuration if sub decision is made after dry-run is filmed; SAT-10 video becomes invalid for the actual shipped configuration |
| Non-technical crew assembly -- installation guide must be complete and photo-documented before SAT-9 | SAT-9, Section 7 | If SAT-9 is rushed because the guide is not ready, failures in the dry-run cannot be distinguished from guide failures vs hardware failures |
| Enclosure sizing dependency on Lucille has no stated deadline | A-3, Scope Analysis | Luca cannot start enclosure fabrication until dims are confirmed; if Lucille is late, enclosure build compresses into the same window as firmware testing |
| 12-component international shipment with no crating spec | Section 7 | Fiberglass horns and SD card audio content are fragile; no crating guidance increases shipping damage risk |
| SAT-7 is a 4-hour proxy for 11-hour operation | SAT-7 | Thermal failures that manifest at hour 6-8 (router overheat, capacitor heat soak) will not be caught before festival deployment |
