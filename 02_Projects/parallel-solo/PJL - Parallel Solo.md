---
date created: 2026-06-15
tags: [project-log, parallel-solo]
category: Project Log
project: "Parallel Solo"
---

# PJL - Parallel Solo

## 2026-06-23

### Bison Installation — Planning, Touch Architecture, Firmware (OTA + Web Config)

**Touch sensor architecture:**
- TTP223 (original D-8) tested -- problematic. No solderable pads for cable extension; any nearby cable or metal triggers sensor. Not viable for large metal sculpture with long electrode runs.
- Two candidates to test before locking: (1) ESP32 native capacitive touch on GPIO 4 (T0) -- wire runs to electrode by design, threshold software-tunable; (2) TTP223B -- test if configurable sensitivity fixes false-trigger issue.
- Created `bison-firmware/bench-test/bench-sequencer/bench-sequencer-touch-test.ino` -- prints raw `touchRead(4)` value continuously, flags TOUCHED vs idle at configurable `TOUCH_THRESHOLD` constant.
- D-8 in plan marked pending update until architecture is locked.

**Project understanding corrections:**
- Lucille makes physical bison structure; Luca delivers tested BRAIN unit + installation guide.
- META crew (JB + Ruts) does physical installation in SF warehouse per SAT-9.
- Removed incorrect Phase 3b "Installation into bison at Lucille's" from plan -- that work is done by META crew, not Luca.
- Fiberglass capacitive test added as standalone section (week of June 30): bring `bench-sequencer-touch-test.ino` + sequencer ESP32 to Lucille's, test touchRead through actual fiberglass, record threshold. If signal blocked: surface-mount pad or drill-through decision with Lucille before BRAIN build.
- Lucille dry run is not a prerequisite for BRAIN assembly. Correct sequence: firmware done (this week) → fiberglass test at Lucille's (week of June 30) → BRAIN box arrives (~July 4) → BRAIN assembly + bench test (1 day) → ship to SF → META crew installs → SATs.

**Schedule locked:**
- This week: Phase 1+2 firmware (touch test, lock architecture, production sequencer AP mode, WLED preset, integration test) -- 1 full day
- Week of June 30: fiberglass capacitive test at Lucille's
- ~July 4: BRAIN box arrives (last component; all others on hand)
- ~July 5–7: BRAIN assembly + bench test (1 day)
- ~July 8–12: SAT-1 through SAT-8 (SAT-7 needs full dedicated day)
- July 10: JB audio hard deadline; escalate July 5 if not received
- July 15: Luca's hard ready/packed date
- July 21: ships

**Plan restructured** (`02_Projects/parallel-solo/plans/2026-06-15/PL - Outside Lands Bison Installation.md`):
- Removed incorrect "Lucille Onsite Dry Run (gates Phase 3)" section
- Phase 3 renamed to "BRAIN Assembly + Bench Test" -- no Lucille prerequisite
- Added "Fiberglass Capacitive Test" section between Phase 2 and Phase 3 (T13b, week of June 30)
- Phase 4 (acceptance testing) updated: SATs run on assembled BRAIN bench unit
- Timeline anchor updated with above schedule
- Work log entries added for June 23 TTP223 test and audio bench test

**PIC updated** (`01_Notes/Pickups/PIC - Bison Bench Test.md`):
- Status section updated to reflect audio bench test complete, TTP223 problematic, native touch next
- Task list rewritten with three blocks: this week / ~July 4 / ~July 8–12
- Critical notes updated: added AP mode note, fiberglass risk, correct sequence

**OTA + web config added to firmware:**
- `bison-firmware/sequencer/sequencer.ino`: added `ArduinoOTA`, `WebServer`, `Preferences`. Config page at http://192.168.4.1 (connect to bison-ap). Exposes `audioDurationMs` (NVS key: `audioDur`, default 30000ms). OTA hostname: `bison-sequencer`, password: `bison2026`. Partition required: Minimal SPIFFS (1.9MB APP with OTA).
- `bison-firmware/bench-test/bench-sequencer/bench-sequencer.ino`: same additions. Config page at device IP (STA mode, printed to serial). Exposes `audioDurationMs` (NVS: `audioDur`) and `wledOffDelayMs` (NVS: `wledOff`), both default 22000ms. OTA hostname: `bison-bench-seq`, password: `bison2026`. Same partition scheme required.
- Both save to NVS via Preferences library; values survive power cycle. First boot uses hardcoded defaults; saved values take over on subsequent boots.
- WLED preset constants and WiFi credentials remain hardcoded (require reflash to change).

**Key open items:**
- Touch architecture: lock after testing native touch + TTP223B
- Production sequencer firmware (AP mode, T2–T5): not yet written
- WLED preset (T11): not yet designed
- JB escalation calendar reminder: set for July 5

### Bison Installation — Bench Audio Firmware

**Goal:** Get bench-test audio ESP32 playing a WAV file via PCM5102MK DAC, triggered by UART 'T' byte from sequencer ESP32. No scratch noise at EOF.

**Hardware confirmed working:**
- ESP32-WROOM-32E (Freenove 38-pin, no PSRAM), Arduino core 3.3.10
- PCM5102MK DAC module (BCK=GPIO26, LRC=GPIO25, DIN=GPIO22, VCC=3.3V)
- SD card module (SCK=GPIO18, MISO=GPIO19, MOSI=GPIO23, CS=GPIO5, VCC=5V — must be 5V)
- UART trigger: GPIO16 RX from sequencer ESP32 GPIO17 TX
- SD card: FAT32, 16GB SanDisk Extreme

**Library path taken:**
- Started on ESP32-audioI2S v3.4.6 — requires PSRAM; ESP32-WROOM-32E has none
- Patched PSRAM check, reduced buffer sizes (m_outbuffSize, AudioBuffer, m_samplesBuff48K) — hit cascading OOM (delay_r, m_httpRespHdrBuff); abandoned
- Downgraded to v2.0.0 — designed for no-PSRAM ESP32

**v2.0.0 compiler fixes (C:\Users\lucas\Documents\Arduino\libraries\ESP32-audioI2S-master\src\aac_decoder\):**
- `aac_decoder.cpp`: Changed `int val` → `int32_t val` in UnpackQuads, UnpackPairsNoEsc, UnpackPairsEsc, DecodeOneScaleFactor, DecodeOneSymbol (5 sites)
- `aac_decoder.cpp`: Changed `unsigned int Get32BitVal(unsigned int*)` → `uint32_t Get32BitVal(uint32_t*)` (definition)
- `aac_decoder.h` line 501: Changed declaration to match `uint32_t Get32BitVal(uint32_t*)`
- `aac_decoder.cpp` line 4491: Changed `DecodeHuffmanScalar` definition 3rd param `uint32_t bitBuf` → `unsigned int bitBuf` to match header (Xtensa: unsigned int ≠ uint32_t at ABI level — linker mismatch)
- `Audio.cpp` `stopSong()`: Removed `i2s_zero_dma_buffer()` call — it was clearing queued silence prematurely

**Audio.ino API fix:** v3.4.6 used `Audio::audio_info_callback` lambda; v2.0.0 uses global `void audio_info(const char*)` function — updated sketch.

**EOF scratch noise root cause + fix:**
- Root cause: After audio EOF, DMA descriptor ring had stale/garbage data in the underrun window; PCM5102 auto-mute briefly released and played it as static
- Fix A: `i2s_zero_dma_buffer(I2S_NUM_0)` called inside `audio_eof_mp3` callback — zeroes all DMA descriptors immediately after stopSong(), before underrun window opens
- Fix B: Looping silence file (`/silence.wav`, 48kHz stereo 16-bit, 5s) plays on startup and after each EOF via `audio.setFileLoop(true)` — keeps I2S BCLK/LRCK alive so PCM5102 never loses clock reference
- Both required: DMA zeros cover the underrun window; silence file prevents clock stall between plays

**SD card contents (final):**
- `/benchtest.wav` — 48kHz stereo 16-bit, ~30s
- `/silence.wav` — 48kHz stereo 16-bit, 5s (generated in Audacity: Generate → Silence)

**Sketch:** `02_Projects/parallel-solo/bison-firmware/bench-test/bench-audio/bench-audio.ino`

**Outcome:** Bench test complete. Audio plays cleanly on trigger; zero scratch at EOF. Bench-sequencer triggers audio via UART and WLED via UDP.

**Next:** Solder ESP32s onto breakout boards → workshop test with real SK6812 LEDs via WLED.

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
