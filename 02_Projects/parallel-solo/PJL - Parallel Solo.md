---
date created: 2026-06-15
tags: [project-log, parallel-solo]
category: Project Log
project: "Parallel Solo"
---

# PJL - Parallel Solo

## 2026-06-15

### Outside Lands Bison Installation — Spec + Review

**Spec created:** `02_Projects/parallel-solo/specs/2026-06-15/SPC - Outside Lands Bison Installation.md`

Source material: meeting with creative team 2026-06-14 + Q&A doc (`bison questions and answers jun152026.txt`). Spec tier: comprehensive (new interactive system spanning touch sensing, audio playback, and addressable lighting across three ESP32 nodes, with a physical shippable structure).

**Architecture locked in spec:**
- Sequencer ESP32 (touch input, UDP orchestration, retrigger timer)
- WLED ESP32 (SK6812 RGBWW 5V via WLED firmware)
- Audio ESP32 (SD card module + 3.5mm/RCA stereo output)
- All three nodes on dedicated WiFi LAN (router inside BRAIN enclosure)
- BRAIN = weathersealed box hard-mounted in Bison HEAD, contains 5V 350W Meanwell PSU + all ESP32s + fan + all I/O ports; enclosure already approved by Lucille

**Review run:** `/review-spec` → 3-agent team → `02_Projects/parallel-solo/reviews/2026-06-15/ARE - Outside Lands Bison Installation Spec Review.md`
Gate: CONDITIONAL (9/12) — 0 Critical issues, 5 clarifications needed.

**Clarifications answered (CL-1 through CL-5):**
- CL-1: Retrigger lock → hard-coded timer = audio file duration (named constant, must update with audio file)
- CL-2: Sub fallback → monitors-only is accepted; NFR-6 evaluated against monitors-only config
- CL-3: BRAIN already sized and approved by Lucille; lives in Bison HEAD not body; coordinate LED paths and head assembly sequence with Lucille during build
- CL-4: SAT-9 assemblers → JB and Ruts (META crew) in SF warehouse
- CL-5: JB is physically onsite; audio content scope only; hardware issues handled via guide with Luca as remote escalation

**Improvements applied to spec (12 total):**
- FR-4: Named-constant retrigger timer mechanism added
- FR-11: Audio format constraints (stereo WAV/MP3, 44.1/48kHz, 16-bit min; duration confirmation before firmware)
- FR-12b: Sub fallback clause (monitors-only accepted; NFR-6 evaluated accordingly)
- FR-19: Fan rain protection tightened — downward-facing louvers or 180° baffle duct; validated by 250ml water pour test at 45°
- SAT-8: Updated to include water pour test as acceptance criterion
- SAT-9: Named META crew (JB/Ruts) as assemblers; SF warehouse
- Section 7 Deliverables: Added crating specification and packing guide
- SAT-7: Added proxy note — 4-hour test accepted for 11-hour day; thermal plateau required at hour 4
- A-1: JB escalation clause added (July 5 escalate; July 10 ship with placeholder + SD update procedure)
- A-3: BRAIN in head; Lucille coordinates LED paths and cable pass-throughs; head assembly sequence to confirm
- A-7: JB onsite scope clarified (audio only); Ruts named; Luca = remote hardware escalation via WhatsApp
- Constraints/Hardware: BRAIN enclosure section added with full hardware inventory

**Skipped improvements:** W5 (WiFi RSSI prototype test) — skipped by Luca.

**PIC closed:** `01_Notes/Pickups/PIC - Your First Spec.md` → status: closed

### Outside Lands Bison Installation — Plan Creation + Spec Updates

**Plan created:** `02_Projects/parallel-solo/plans/2026-06-15/PL - Outside Lands Bison Installation.md`
- 34 tasks, Standard tier ceremony
- 5 phases (bench firmware → full playback → BRAIN assembly → acceptance testing → docs/ship) + Lucille onsite dry run milestone gating Phase 3
- Decision log: 10 decisions captured (D-1 through D-10)
- Critical path: JB audio delivery gates T30; Lucille onsite dry run gates T14

**Spec updates after plan:**

FR-13b added: Horn tips include addressable LEDs (SK6812 bare pixel strip) on same WLED string as body. Lucille must preserve hollow wire routing path through each horn before fiberglass sealed — cannot retrofit.

FR-13 updated: LED strips mount in belly (interior underside) of lattice ribs in aluminum channels with milky white diffuser. 3–6 x 1m segments. LED density open decision: 144 LED/m preferred (lower per-LED power at partial brightness amber/warm-white blend = smoother gradients + less heat); 60 LED/m fallback if 144 not in Parallel stock. Power concern at 144 LED/m x 6m moot — system never runs all RGBWW channels at full output.

FR-20b added: Full TRUE1 power topology. Foot: Neutrik TRUE1 chassis inlet (mains in) + TRUE1 chassis outlet (PA power out). BRAIN: TRUE1 chassis inlet → Meanwell PSU. Custom TRUE1-to-Edison pigtail cable + TRITAPs ship with installation for PA daisy chain. Added to Section 7 deliverables.

T13b added (plan): Lucille onsite dry run task — brings working bench firmware to Lucille's workspace, locks layout decisions (LED belly positions, touch sensor placement on snout/horns, horn wire routing confirmation, head assembly sequence, cable pass-throughs). Gates T14 (BRAIN assembly).

A-3 updated (spec): Horn wire routing path added as pre-fabrication coordination requirement with Lucille.

### Outside Lands Bison Installation — Firmware + Bench Test

**Single TTP223 architecture confirmed (Luca 2026-06-15):**
Snout + both horn copper electrodes wired in parallel to ONE TTP223 pad input. Single digital output → sequencer GPIO 4. No zone differentiation in firmware — any touch triggers same sequence.

**Files updated:**
- `bison-firmware/sequencer/sequencer.ino`: removed `PIN_TOUCH_SNOUT`/`PIN_TOUCH_HORNS`, replaced with `PIN_TOUCH = 4`. `touchActive()` simplified to one `digitalRead(PIN_TOUCH)`. `setup()` down to one `pinMode()`.
- `specs/2026-06-15/SPC - Outside Lands Bison Installation.md`: FR-1 rewritten — single TTP223, all three electrodes joined at pad input, one GPIO. FR-2 rewritten — explicitly states ONE TTP223, ONE GPIO, no zone differentiation.
- `plans/2026-06-15/PL - Outside Lands Bison Installation.md`: T17 updated — "ONE TTP223 module — snout and both horn electrodes wired together to its pad input, single wire to sequencer GPIO 4."

**Compile results:**
- `sequencer.ino`: 1,034,116 bytes (78% / 1,310,720) flash, 48,504 bytes (14%) RAM. Clean.
- `audio.ino`: failed at default partition (102%). Fixed: Arduino IDE → Tools → Partition Scheme → "No OTA (2MB APP / 2MB SPIFFS)". Recompile: 1,346,798 bytes (64% / 2,097,152) flash, 43,792 bytes (13%) RAM. Clean. Root cause: ESP32-audioI2S bundles decoders for MP3/AAC/FLAC/WAV. OTA not needed for festival install.
- **Note for future sessions:** audio.ino requires "No OTA (2MB APP / 2MB SPIFFS)" partition in Arduino IDE. Board-level IDE setting — not stored in .ino file. Must reapply on any new machine or fresh Arduino install.

**Bench test sketch created:**
`bison-firmware/bench-test/bench-sequencer/bench-sequencer.ino`
- Adapted from `C:\Users\lucas\Documents\Arduino\SKetchtriggertest\finalsketchenlieu\finalsketchenlieu.ino`
- Differences from production: WiFi STA mode ("Parallel" / "parallelo"); INPUT_PULLUP button GPIO 4 (LOW=pressed vs TTP223 HIGH=touched); no UART TX; `WLED_IP` placeholder on line 37 awaiting home network IP
- `WLED_PRESET = 1` — must match a saved preset in home WLED instance

**Pending before bench test (Luca):**
- Find WLED ESP32 IP on home network → update `bench-sequencer.ino` line 37
- Design and save WLED preset 1 on home WLED instance (T11 — no bison hardware needed)
