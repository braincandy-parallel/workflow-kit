---
date created: 2026-06-15
tags: [plan, parallel-solo, outside-lands]
category: Plan
status: Draft
source: "[[SPC - Outside Lands Bison Installation]]"
ceremony_tier: standard
completed: 0
total: 34
---

# PL - Outside Lands Bison Installation

Implementation plan for the Outside Lands 2026 Bison interactive art installation. Translates [[SPC - Outside Lands Bison Installation]] into a sequenced build across 5 vertical phases. Review artifacts: [[ARE - Outside Lands Bison Installation Spec Review]].

**Critical path:** JB audio delivery (July 10 hard deadline) gates Phase 5 T30. All other phases are independent of JB.

**Ship date:** July 21, 2026. Ready date: July 15-19. **Development start: June 22, 2026** (gear delivered by end of week of June 15).

---

## Decision Log

| # | Decision | Rationale | Source |
|---|----------|-----------|--------|
| D-1 | Retrigger lock = hard-coded timer (AUDIO_DURATION_MS constant) | Simpler than return-channel; constant updated when audio changes | CL-1 |
| D-2 | Sub fallback = monitors-only accepted; NFR-6 evaluated against monitors-only | Budget gate unresolved at spec time | CL-2 |
| D-3 | BRAIN enclosure in Bison HEAD, already sized by Lucille | Head approved; coordinate LED paths + head assembly sequence with Lucille | CL-3 |
| D-4 | SAT-9 assemblers = JB + Ruts (META crew) in SF warehouse | Named crew; Luca remote escalation | CL-4 |
| D-5 | JB onsite = audio content scope only; hardware via guide + Luca remote | JB is not an electronics technician | CL-5 |
| D-6 | Fan vent = downward-facing louvers or 180° baffle; validated by 250ml pour test | Specific testable standard replacing "mesh + baffle or similar" | W1 |
| D-7 | No router. Sequencer ESP32 is WiFi AP. Audio trigger = UART wire. WLED trigger = UDP to Sequencer AP. | Removes router as point of failure. Audio ESP32 WiFi-free = cleaner I2S audio (no WiFi interrupt interference). Two independent trigger paths. | 2026-06-15 |
| D-8 | Touch sensor: TTP223 module, pad input wired to copper lattice on fiberglass. Digital HIGH/LOW output to ESP32 GPIO. | TTP223 already ordered. Copper lattice = aesthetic element + touch electrode. Simple digitalRead() in firmware — no calibration. Ruts approval needed before lattice fabrication, not before firmware dev. | 2026-06-15 |
| D-9 | Spare parts onsite: recommend minimal kit (fuses, connectors, 1x ESP32) | Pending client (Superfly) decision | Spec open decision |
| D-10 | Horn tips include addressable LEDs (SK6812 bare pixel strip) wired on same WLED string as body | Render shows amber glow in horn tips; confirmed by Luca 2026-06-15 | Render review |

---

## Firmware File Structure

All firmware lives in a single `bison-firmware/` repo (create at project start):

```
bison-firmware/
├── sequencer/sequencer.ino     ← Sequencer ESP32 (touch, retrigger, UDP broadcast)
├── audio/audio.ino             ← Audio ESP32 (SD card playback, UDP listener)
├── wled/presets.json           ← WLED preset export (sequence config)
├── network/                    ← Router config notes, static IP assignments
│   └── ip-assignments.md
└── docs/                       ← Wiring diagram source, installation guide
```

Network assignments (Sequencer is AP — no router):
- Sequencer ESP32 AP gateway: 192.168.4.1 (Sequencer itself)
- WLED ESP32: 192.168.4.11 (static lease on Sequencer AP)
- Audio ESP32: no WiFi (UART only)

---

## Phase 1: Touch-to-UDP Working (Bench Prototype)

**User-testable outcome:** Touch the snout sensor on the bench → serial monitor on both the audio and WLED ESP32s shows UDP packet received, all within 250ms.

**Gate to Phase 2:** T8 passes on 5 consecutive touches.

| # | Task | Status | Acceptance Criteria | Deps | Files | Notes |
|---|------|--------|---------------------|------|-------|-------|
| 1 | Bench-test TTP223 with copper surrogate pad | todo | Wire TTP223 output to ESP32 GPIO; connect copper foil surrogate to TTP223 pad input. Serial monitor shows stable HIGH on contact, LOW at rest. No false triggers in 60-second idle. Verify TTP223 A/B pad config (default: momentary, active HIGH). | -- | `sequencer/sequencer.ino` | TTP223 outputs clean digital signal — no threshold calibration needed. Copper foil = bench surrogate for the lattice. Check TTP223 A/B solder pads: A=open, B=open gives momentary active-high output, which is what we want. |
| 2 | Write sequencer firmware: TTP223 GPIO read + debounce | todo | Serial monitor shows "TOUCH DETECTED" only on intentional contact. No spurious triggers in 60-second idle test. | 1 | `sequencer/sequencer.ino` | FR-1, FR-2, FR-3. digitalRead() on TTP223 output pin — no analog read or threshold needed. Include software debounce constant (short, TTP223 already debounces in hardware). |
| 3 | Write sequencer firmware: retrigger lock timer | todo | Touch during active lock window prints "LOCKED – ignoring" to serial. Lock clears after AUDIO_DURATION_MS ms. Set AUDIO_DURATION_MS = 30000 (placeholder; update per FR-11 once JB delivers). | 2 | `sequencer/sequencer.ino` | FR-4. AUDIO_DURATION_MS is the named constant per D-1. Document update procedure in a code comment. |
| 4 | Write sequencer firmware: UDP broadcast to audio + WLED IPs | todo | Serial monitor shows "UDP SENT to 192.168.4.11" and "UDP SENT to 192.168.4.12" within 10ms of touch detection. | 3 | `sequencer/sequencer.ino` | FR-5. Use static IPs from network/ip-assignments.md. |
| 5 | Configure Sequencer ESP32 as WiFi AP; assign static IP to WLED ESP32 | todo | Sequencer AP broadcasts SSID. WLED ESP32 connects as client. Sequencer pings WLED IP successfully. AP survives power cycle with same credentials. | -- | `sequencer/sequencer.ino`, `network/ip-assignments.md` | FR-6, A-4. No router. Sequencer is AP. Audio ESP32 has no WiFi — skip from this test. Document AP SSID, password, and WLED static IP in ip-assignments.md. |
| 6 | [P] Write stub audio ESP32 firmware: listen on UART + echo to serial | todo | Send UART byte from Sequencer TX → Audio RX. Audio serial monitor prints "TRIGGER RECEIVED" within 10ms. Audio ESP32 has no WiFi configured. | 3 | `audio/audio.ino` | FR-9, A-4. UART only — no WiFi, no UDP on audio node. Confirms wired trigger path. |
| 7 | [P] Write stub WLED config: join Sequencer AP + confirm UDP realtime reception | todo | WLED connects to Sequencer AP. WLED debug log shows UDP realtime packet received within 250ms of touch event. | 4, 5 | `wled/presets.json` | Full sequence in Phase 2. Use WLED's built-in UDP debug output to confirm packet receipt. |
| 8 | Integration test: touch → UART (audio) + UDP (WLED) → both nodes | todo | (1) Touch → Audio stub prints "TRIGGER RECEIVED" within 250ms. (2) Touch → WLED UDP received within 250ms. Both on 5 consecutive tests. | 6, 7 | -- | SAT-1 precursor. NFR-1. Two independent paths tested separately then simultaneously. |

---

## Phase 2: Full Sequence Playback

**User-testable outcome:** Touch the sensor → LEDs run the full WLED animation sequence AND audio plays from SD card simultaneously. System returns to idle (LEDs off, no audio) at sequence end.

**Gate to Phase 3:** T13 passes; retrigger lock holds during active sequence.

| # | Task | Status | Acceptance Criteria | Deps | Files | Notes |
|---|------|--------|---------------------|------|-------|-------|
| 9 | Complete audio ESP32 firmware: SD card playback on UDP trigger | todo | Touch event → audio plays from SD card through 3.5mm/RCA output. Playback duration matches file length. File: placeholder WAV (≥30s). | 8 | `audio/audio.ino` | FR-9, FR-10, FR-11. Confirm WAV decode library + SD card module work together before integration. |
| 10 | Load placeholder audio file to SD card; verify RCA output | todo | Connect RCA output to Luca's test PA. Audio audible and undistorted at intended volume. No clipping. | 9 | SD card file | FR-12. File must be stereo WAV, 44.1kHz or 48kHz, 16-bit. Duration must match AUDIO_DURATION_MS placeholder value. Update AUDIO_DURATION_MS if duration differs. |
| 11 | [P] Design and configure WLED sequence (body + horn tips) | todo | WLED sequence runs to completion on manual trigger: body channels + horn tip pixels animate. Warm/amber/warm-white blend at partial brightness — no full-output RGBWW. Gradient and color quality smooth with no visible banding. Save as named preset, export to wled/presets.json. Duration matches or is slightly shorter than audio file. | 5 | `wled/presets.json` | FR-13b, FR-15. Preferred strip: 144 LED/m (lower per-LED power at target brightness = smoother blend + less heat). Use 60 LED/m if 144/m not in Parallel stock. Density decision locks before T16. |
| 12 | [P] Map WLED UDP realtime trigger from sequencer | todo | Sequencer touch → WLED starts preset sequence automatically (no manual trigger). Confirm WLED UDP realtime protocol port/format matches sequencer UDP packet. | 11, 8 | `sequencer/sequencer.ino`, `wled/presets.json` | FR-5, FR-15. WLED UDP realtime uses specific packet format; verify sequencer output matches. |
| 13 | Integration test: full sequence — touch → LED + audio → idle | todo | (1) Touch → LEDs + audio start simultaneously within 250ms. (2) Touch during active sequence → no restart (SAT-3). (3) Sequence ends → LEDs off + no audio within 5s (SAT-4). Verify on 5 consecutive full-sequence cycles. | 10, 12 | -- | Covers SAT-2, SAT-3, SAT-4 precursors. |

---

## Lucille Onsite Dry Run (gates Phase 3)

Bring working bench firmware to Lucille's workspace. Test the full touch-to-sequence on or near the physical structure. This session locks all layout decisions that Phase 3 depends on. Do not start T14 until this session is complete.

**Outputs required from this session before Phase 3 starts:**
- Touch sensor placement confirmed on snout and horns (sensor type validated against fiberglass)
- LED channel belly positions confirmed (how many segments, spacing along ribs)
- Horn wire routing path confirmed accessible
- Cable pass-through locations through body panels confirmed
- Head assembly sequence confirmed (open vs closed during sensor wire routing)
- Any structural constraints or surprises documented

| # | Task | Status | Acceptance Criteria | Deps | Files | Notes |
|---|------|--------|---------------------|------|-------|-------|
| 13b | Lucille onsite dry run — layout confirmation | todo | All 5 outputs above are documented. LED belly positions locked. Touch sensor placement on snout + horns confirmed viable. Head assembly sequence confirmed. Any structural surprises captured before BRAIN build starts. | 13 | `docs/layout-decisions.md` | This session influences trigger design, LED segment count, and installation guide assembly sequence. Bring: bench firmware running on 3 ESP32s, 1m test LED strip in channel with diffuser, test touch sensor, laptop. |

---

## Phase 3: BRAIN Assembly + Physical Integration

**User-testable outcome:** All electronics mounted in BRAIN enclosure inside Bison head; touch sensors on snout and horns; LED strips in aluminum channels; all cables labeled; fan vent passes 250ml water pour test.

**Prerequisite:** T13b (Lucille onsite dry run) must be complete. All layout decisions are locked coming out of that session. Horn wire routing path must be confirmed accessible before Lucille seals the fiberglass horns — this cannot be retrofitted after sealing.

**Gate to Phase 4:** Full sequence triggers correctly from snout and horns with BRAIN assembled and mounted.

| # | Task | Status | Acceptance Criteria | Deps | Files | Notes |
|---|------|--------|---------------------|------|-------|-------|
| 14 | Assemble BRAIN enclosure + wire TRUE1 power distribution | todo | (1) All BRAIN components secured. Power on via TRUE1 chain: foot inlet → internal AC run → BRAIN TRUE1 inlet → Meanwell → all ESP32s boot, join WiFi, ping each other. (2) Foot TRUE1 outlet wired and labeled. (3) Custom TRUE1-to-Edison pigtail built and tested (plugs into foot outlet, Edison end feeds TRITAP, TRITAP powers PA). (4) No loose connections after shake test. | 13b | -- | FR-20b. TRUE1 on foot (inlet + outlet) and BRAIN (inlet). Build custom pigtail cable: TRUE1 male → Edison female. Ship with 1-2 TRITAPs. |
| 15 | Install cooling fan with downward-facing louver/baffle duct | todo | (1) Fan spins at full power on 5V. (2) Water pour test: 250ml poured at 45° over vent — no water reaches fan blade or enclosure interior (SAT-8 criterion). | 14 | -- | FR-19, D-6. Choose between downward-facing louvers or 180° baffle. Document chosen solution with photo. |
| 16 | [P] Mount SK6812 strips in belly channels + wire horn LED strips | todo | (1) Body LED strips (3–6 x 1m segments in belly aluminum channels with milky diffuser) light correctly on WLED trigger, no dead zones, diffuser producing even wash (not point-source visible). (2) Both horn tip pixels light on WLED trigger. All connections secure, no exposed solder points. | 14 | -- | FR-13, FR-13b, FR-16. Belly placement = interior underside of lattice ribs. Start with 3 segments; add 3 more if coverage is insufficient. Horn LEDs: bare SK6812 pixels through Lucille's preserved horn wire path. IP65 minimum all strips + connections. |
| 17 | [P] Install TTP223 touch sensor + copper electrodes on snout + both horn zones; wire all to single GPIO | todo | Touch snout → serial shows TOUCH. Touch left horn → TOUCH. Touch right horn → TOUCH. No false triggers when hands are 30cm away. | 14 | `sequencer/sequencer.ino` | FR-1, FR-2. ONE TTP223 module — snout and both horn electrodes wired together to its pad input, single wire to sequencer GPIO 4. Confirm fiberglass does not fully block capacitive signal before final mounting. If blocked, escalate to Luca before drilling. |
| 18 | Label all cables; confirm wiring diagram matches physical labels | todo | Every cable has a label at both ends. Wiring diagram (docs/wiring-diagram) lists every labeled cable. Labels survive 60-second tug test (no slippage). | 16, 17 | `docs/wiring-diagram` | FR-22. Use heat-shrink label sleeves or P-touch labels. Take photos for installation guide. |
| 19 | Verify full sequence from mounted sensors (snout + both horns) | todo | Touch snout → full sequence (LEDs + audio) within 250ms. Touch left horn → same. Touch right horn → same. Retrigger lock holds during active sequence. | 15, 16, 17, 18 | -- | FR-3. Final physical integration check before formal SAT tests. |

---

## Phase 4: Acceptance Testing

**User-testable outcome:** All SAT-1 through SAT-8 criteria pass. System is verified ready for pre-ship dry-run video.

**Gate to Phase 5:** All SATs pass. Thermal readings at SAT-7 hour 4 show stable plateau.

| # | Task | Status | Acceptance Criteria | Deps | Files | Notes |
|---|------|--------|---------------------|------|-------|-------|
| 20 | SAT-1/2: Touch response timing — snout + both horns | todo | Stopwatch from touch to first LED/audio output ≤250ms on 5 consecutive tests per zone (15 total tests). All three zones pass. | 19 | -- | SAT-1, SAT-2. |
| 21 | SAT-3: Retrigger lock | todo | Touch repeatedly during active sequence → sequence plays to completion uninterrupted on 5 consecutive tests. | 20 | -- | SAT-3. |
| 22 | SAT-4: Idle state return | todo | After sequence end, observe system for 60 seconds → LEDs off, no audio, within 5 seconds of sequence end. Verify on 3 consecutive cycles. | 21 | -- | SAT-4. FR-7, FR-8. |
| 23 | SAT-5: Audio quality at 3 meters | todo | Play full sequence at intended volume. Listen at 3m. Pass: audio clearly audible, undistorted, no clipping or dropouts. If using monitors-only (no sub), document SPL result against NFR-6. | 22 | -- | SAT-5, NFR-6, D-2. |
| 24 | SAT-6: Lighting sequence completeness | todo | Touch → WLED sequence runs to completion. No flickering, no dead zones. Sequence matches intended design (warm/amber/white burst). | 23 | -- | SAT-6. |
| 25 | SAT-7: 4-hour continuous power test | todo | Power on at T=0. Trigger sequence every 10 minutes. At T=4h: no restarts, no overheating, fan operational. **Thermal check at T=4h: readings must be on stable plateau (not still rising).** Record thermal readings at T=1h, T=2h, T=3h, T=4h. | 24 | -- | SAT-7. NFR-2. Rising trend at T=4h = fail; investigate heat source before proceeding. |

---

## Phase 5: Documentation + Ship

**User-testable outcome:** All documentation complete. JB audio loaded (or placeholder with SD update procedure). SAT-9 crew dry-run passes. Pre-ship video filmed. Packed and shipped by July 21.

**JB audio dependency:** T30 cannot start until JB delivers final audio files. If not received by July 5, escalate per A-1. If not received by July 10, ship with placeholder and hand META crew the SD update procedure.

| # | Task | Status | Acceptance Criteria | Deps | Files | Notes |
|---|------|--------|---------------------|------|-------|-------|
| 26 | Write installation guide — assembly section | todo | A person unfamiliar with the installation can identify every component and connection point from photos + text alone. Peer review: Luca reads aloud without looking at the physical system and can describe every step. | 25 | `docs/installation-guide.md` | NFR-4. Sections: assembly sequence with photos, cable connection guide, power-on/off procedure. |
| 27 | Write installation guide — troubleshooting + rain plan | todo | Troubleshooting tree covers: no power, no audio, no LEDs, no touch response, WiFi not connecting. Rain plan: power-off procedure, tarp placement. | 26 | `docs/installation-guide.md` | A-6. Troubleshooting should be solvable by JB/Ruts without calling Luca for common issues (per D-5). |
| 28 | Write installation guide — post-event storage/boxing | todo | Step-by-step boxing guide with which component goes in which crate. References crating spec. | 27 | `docs/installation-guide.md` | |
| 29 | Write wiring diagram + spare parts list | todo | Wiring diagram lists every labeled cable with source and destination. Spare parts list includes item, sourcing link (Amazon/AliExpress/Adafruit), quantity. | 18 | `docs/wiring-diagram`, `docs/spare-parts.md` | FR-22, NFR-5. Recommend: fuses, connectors, 1x ESP32 of each type, spare SD card. |
| 30 | Load final JB audio files; update AUDIO_DURATION_MS; re-run SAT-1 + SAT-3 | todo | Final audio file loaded to SD card. AUDIO_DURATION_MS constant updated to exact file duration. Re-run SAT-1 (timing ≤250ms) and SAT-3 (retrigger lock holds full sequence). | 29 | `audio/audio.ino`, SD card | FR-9, FR-11, D-1. If JB not delivered by July 10: ship with placeholder; document SD card swap procedure in a one-page addendum for META crew. |
| 31 | Write crating spec and packing guide | todo | Crating spec covers: padding requirements per component (fiberglass horns = foam wrap; BRAIN module = foam-lined case; LED channels = bubble wrap), orientation markings (THIS SIDE UP), fragile labels, moisture protection for BRAIN module. | 25 | `docs/crating-spec.md` | W3 improvement. Per spec: 12 components total (4 legs, head, horns, body x4, steel base). |
| 32 | SAT-9: META crew guide-only assembly dry-run | todo | JB or Ruts assemble tech module and connect all cables using only the installation guide in SF warehouse. Pass: full assembly completed without verbal guidance from Luca. Luca available remotely. Any guide gaps found → update guide before filming. | 26, 27, 28, 30 | -- | SAT-9. Must complete by July 18 to allow guide revisions before video. |
| 33 | Film pre-ship dry-run video; pack + ship | todo | Video shows SAT-1 through SAT-8 all passing in sequence. Video shared with Superfly (client) before ship. Pack all 12 components per crating spec. Ship by July 21. | 31, 32 | -- | SAT-10. If any SAT fails on video day: fix and re-film. Do not ship without passing video. |

---

## Risk Register

| Risk | Phase | Likelihood | Impact | Mitigation |
|------|-------|-----------|--------|------------|
| Capacitive sensor blocked by fiberglass resin | Phase 1 T1 | Medium | High | Test before mounting. If blocked: surface-mount pad or drill-through sensor (needs Lucille approval). Decide at T1 before any mounting. |
| JB audio late (after July 10) | Phase 5 T30 | Medium | High | Ship with placeholder; hand META crew SD update procedure per A-1. Placeholder must be same duration as expected final file. |
| UART wire break inside BRAIN | Phase 3 T17 | Low | Medium | Single TX→RX wire between Sequencer and Audio ESP32. Use quality wire, secure both ends with strain relief. Carry one spare pre-crimped wire in spare parts kit. |
| AUDIO_DURATION_MS mismatch if JB delivers different-duration file | Phase 5 T30 | Medium | Medium | Constant is named; update procedure documented in code. Re-run SAT-1/SAT-3 after every audio file change. |
| Thermal failure after hour 4 (SAT-7 proxy limitation) | Phase 4 T25 | Low | Medium | Thermal plateau check at T=4h is mandatory. If still rising at T=4h, investigate before shipping. |
| LED channel mounting path blocked by body panel design | Phase 3 T16 | Low | High | Coordinate with Lucille before T14. Confirm cable pass-through locations before BRAIN enclosure is fully assembled. |
| SAT-9 guide failures reveal major documentation gaps | Phase 5 T32 | Medium | Medium | Reserve July 18 for guide revisions. Do not schedule T32 on July 19 or later. |

---

## Timeline Anchor

| Milestone | Target Date | Gate |
|-----------|-------------|------|
| Phase 1 + 2 complete (firmware working on bench) | July 1 | Touch-to-sequence working end-to-end |
| Phase 3 complete (BRAIN assembled, sensors mounted) | July 8 | Full sequence triggers from snout + horns |
| Phase 4 complete (all SATs pass) | July 12 | All SAT-1 through SAT-8 pass |
| JB audio hard deadline | July 10 | Escalated July 5 if not received |
| SAT-9 crew dry-run | July 18 | Guide-only assembly passes |
| Pre-ship video + pack | July 20 | All SATs on video |
| Ship | July 21 | Hard deadline |

---

## Work Log

| Date | Task # | Agent | What was done | Notes |
|------|--------|-------|---------------|-------|
