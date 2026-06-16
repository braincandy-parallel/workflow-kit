---
date created: 2026-06-15
tags: [spec, parallel-solo, outside-lands, interactive-art]
category: Spec
status: Draft
source: "Meeting with creative team 2026-06-14; Q&A doc bison questions and answers jun152026.txt"
tier: comprehensive
comprehensive_reason: New interactive system spanning touch sensing, audio playback, and addressable lighting across three ESP32 nodes, with a physical shippable structure, non-technical handoff, and YOY durability requirements.
---

# SPC - Outside Lands Bison Installation

Interactive touch-sensitive Bison sculpture for Outside Lands Music Festival 2026. Triggers synchronized audio and lighting sequences when visitors touch the snout or horns. Ships from Los Cabos to San Francisco. Designed for recurring year-over-year deployment by non-technical crews.

---

## 1) Purpose / Objectives

Create a self-contained, weather-resistant interactive art installation in the form of a Bison sculpture that:
- Responds to human touch on the snout and horns with a synchronized audio and lighting sequence
- Operates reliably across 3 festival days with no technical supervision beyond initial power-on
- Ships from Los Cabos to San Francisco as 12 discrete, crated components
- Can be assembled, operated, and struck by non-technical festival crew following a written installation guide
- Is designed for year-over-year (YOY) redeployment

**Success looks like:**
- A visitor touches the snout or either horn and within 250ms a full audio + lighting sequence begins
- The sequence plays to completion; no re-trigger is possible mid-sequence
- The installation operates for 11+ hours per day without intervention
- Superfly crew assembles the full installation in San Francisco without direct Luca involvement, using only the installation guide
- Client (Superfly) signs off after reviewing the pre-ship dry-run video

---

## 2) Scope (In) and Exclusions (Out)

**In scope:**
- Touch sensing system (snout + horns)
- Audio playback system (ESP32 + SD card + RCA output + PA/sub)
- Addressable LED lighting system (SK6812 RGBWW via WLED)
- ESP32 sequencer/controller logic (UDP orchestration)
- Weathersealed tech module inside the Bison body
- All wiring, cabling, connectors, and power distribution inside the module
- Pre-ship dry-run and video documentation
- Installation guide (assembly, wiring, troubleshooting, rain plan, storage/boxing)
- Spare parts list

**Out of scope:**
- Physical structure fabrication (Lucille's scope)
- Floral/foliage decoration (creative team's scope)
- Festival power hookup and extension cabling beyond the Bison's power inlet
- Audio content composition (JB at Silent G Studios)
- On-site logistics (housing, transport, catering)
- Any tech work for years 2027+ (Luca retains exclusive right to offer maintenance, but it is a separate engagement)
- Remote monitoring or wireless app control
- DMX (all lighting via WLED/UDP, not DMX)

---

## 3) Functional Requirements

### 3.1 Touch Sensing

- **FR-1:** The system MUST detect capacitive touch on the snout or either horn via a single TTP223 sensor module. The TTP223 pad input connects to conductive electrodes on all three surfaces (snout + left horn + right horn), with all electrode wires joined at the pad. Any touch anywhere on those surfaces produces a single digital HIGH output to one sequencer ESP32 GPIO pin. Electrode material is Lucille's fabrication decision pending Ruts approval (see A-8).
- **FR-2:** All touch zones (snout + both horn tips) share ONE TTP223 and ONE sequencer GPIO pin. The copper electrodes on all three surfaces are electrically unified at the TTP223 pad input. Firmware reads one pin; zone differentiation is not needed — any touch triggers the same sequence.
- **FR-3:** Touch on ANY active zone (snout or horns) MUST trigger the sequence. All zones are equivalent — they trigger the same sequence at the same intensity.
- **FR-4:** The sequencer MUST ignore touch input while a sequence is actively playing (retrigger delay = duration of full sequence). The sequencer clears the retrigger lock via an internal timer set to the audio file duration, configured as a named constant. This constant MUST be updated whenever the audio file duration changes.

### 3.2 Sequence Orchestration

- **FR-5:** On touch detection, the sequencer ESP32 MUST send a UART trigger to the audio ESP32 (TX→RX wire) AND a UDP packet to the WLED ESP32 simultaneously.
- **FR-6:** The sequencer ESP32 MUST operate as a WiFi access point. The WLED ESP32 connects to this AP as a client. The audio ESP32 has no WiFi — it receives triggers via UART only.
- **FR-7:** The sequencer MUST enforce a locked idle state (all outputs off) when no touch is active and no sequence is playing.
- **FR-8:** The system MUST return to idle state automatically at the end of every sequence.

### 3.3 Audio Playback

- **FR-9:** The audio ESP32 MUST play a pre-loaded audio file from SD card on receipt of a UART trigger from the sequencer. The audio ESP32 has no WiFi connection.
- **FR-10:** Audio output MUST be stereo via the 3.5mm/RCA output to the PA system.
- **FR-11:** Audio content MUST be provided by JB (Silent G Studios) as WAV or MP3 files loaded onto the SD card before shipping. Files MUST be stereo, 44.1kHz or 48kHz, 16-bit minimum. Final file duration MUST be confirmed to Luca before the retrigger timer constant is set in firmware. The system MUST support at minimum one audio file; additional files for future content updates are a nice-to-have.
- **FR-12:** Audio volume SHOULD be set to maximum safe level for the PA system — loud enough to compete with ambient festival noise without distortion.
- **FR-12b:** The PA system MUST include 2x monitor speakers. A subwoofer SHOULD be included if budget permits. If the subwoofer is cut, monitors-only is the accepted fallback and NFR-6 is evaluated against the monitors-only configuration.

### 3.4 Lighting

- **FR-13:** The WLED ESP32 MUST control SK6812 RGBWW 5V addressable LED strips mounted in aluminum channels with milky white diffuser caps along the belly (interior underside) of the Bison lattice body. Segment count: 3–6 x 1-meter segments (final count determined during prototyping based on visual output). LED density: 60 LED/m or 144 LED/m (see Open Decisions — validate 60 LED/m brightness before committing to 144 LED/m).
- **FR-13b:** The WLED ESP32 MUST also control addressable LEDs inside both fiberglass horn tips. Horn LEDs MUST be wired on the same WLED ESP32 string as the body LEDs. Horn LEDs may use bare SK6812 pixel strip or equivalent (aluminum channels will not fit inside fiberglass horn interiors). **Lucille MUST preserve a hollow wire routing path through each horn before the fiberglass is sealed — this cannot be retrofitted after fabrication.**
- **FR-14:** In idle state, all LEDs MUST be off.
- **FR-15:** On receipt of a UDP trigger, WLED MUST execute a pre-programmed sequence (colors, animation, duration). Sequence design is Luca's creative decision during prototyping.
- **FR-16:** LED strips and connections MUST be rated or sealed to IP65 or better for outdoor use.

### 3.5 Power

- **FR-17:** The tech module MUST operate on a single 20A / 120V AC circuit supplied by the festival.
- **FR-18:** Total power draw at peak MUST NOT exceed 2000W.
- **FR-19:** The system MUST include a cooling fan for the electronics enclosure. The fan inlet/outlet MUST use downward-facing louvers or a 180-degree baffle duct such that water entering the vent must travel upward to reach the fan or enclosure interior. Validated by: pour 250ml of water at 45 degrees over the vent — no water reaches the fan blade or enclosure interior.
- **FR-20:** All power connections MUST be accessible via labeled, detachable cables for strike and storage.
- **FR-20b:** Mains AC power MUST enter the Bison at the foot via a Neutrik TRUE1 chassis inlet (panel-mount, IP65). From the foot, AC routes internally to: (1) BRAIN enclosure via a Neutrik TRUE1 chassis inlet on the BRAIN, powering the Meanwell PSU; (2) a Neutrik TRUE1 chassis outlet at the foot for the PA system. The TRUE1 foot outlet connects via a custom TRUE1-to-Edison pigtail cable to a TRITAP, which distributes power to all PA speakers. The festival crew connection workflow: plug festival extension into foot TRUE1 inlet, plug custom TRUE1-to-Edison cable into foot TRUE1 outlet, plug TRITAP onto Edison end, connect all speakers to TRITAP. All connectors MUST be labeled.

### 3.6 Physical Module

- **FR-21:** All electronics MUST be housed in a weathersealed enclosure inside the Bison body, accessible only with a tool (combo lock or impact driver + screws).
- **FR-22:** Every cable MUST be labeled. Matching labels MUST appear in the wiring diagram in the installation guide.
- **FR-23:** The tech module MUST be removable as a unit from the Bison body for transport and storage.

---

## 4) Non-Functional Requirements

- **NFR-1 Latency:** Touch-to-sequence-start latency MUST be under 250ms (perceived as immediate by a festival visitor).
- **NFR-2 Uptime:** The system MUST operate continuously for 11 hours per day (11AM–10PM) without restart or intervention.
- **NFR-3 Durability:** All exposed components (LEDs, sensors, wiring) MUST tolerate outdoor summer conditions in San Francisco (fog, humidity, light wind). IP65 minimum for exposed elements.
- **NFR-4 Assembly:** A non-technical festival crew member MUST be able to install the tech module and connect all cables using only the installation guide — no prior electronics experience assumed.
- **NFR-5 YOY:** Hardware choices MUST favor commodity/replaceable components available from standard distributors (Amazon, AliExpress, Adafruit). No custom or bespoke parts that would be impossible to source in 2027+.
- **NFR-6 Audio level:** System MUST produce sufficient SPL to be clearly audible at 3 meters from the sculpture in a moderately loud festival environment.

---

## 5) Constraints

**Hardware — locked:**
- BRAIN enclosure: weathersealed box hard-mounted in the Bison head. Contains: 5V 350W Meanwell PSU, sequencer ESP32, WLED ESP32, audio ESP32, all touch sensor input ports, all lighting/audio output ports, cooling fan. No router — sequencer acts as WiFi AP. Enclosure size already approved by Lucille.
- Power supply: 5V 350W Meanwell (powers all ESP32s and SK6812 LEDs)
- Sequencer: ESP32 (touch input, UDP orchestration, retrigger timer)
- Lighting controller: ESP32 running WLED firmware
- Audio controller: ESP32 with SD card module + stereo audio output (3.5mm/RCA)
- LEDs: SK6812 RGBWW 5V strip (from Parallel stock, sold to Parallel-solo at internal price)
- Aluminum LED channels: from Parallel stock
- Audio system: 2x PA monitor speakers + subwoofer (sub pending budget). Monitors-only is accepted fallback. Luca's test rig used for prototyping; onsite system assembled by META crew per installation guide.
- Festival ambient audio (not Luca's system): 8" x 4 PA speakers, no sub — separate from the Bison installation.
- Power connectors: Neutrik TRUE1 (IP65) throughout. Foot: TRUE1 chassis inlet (mains in) + TRUE1 chassis outlet (PA power out). BRAIN: TRUE1 chassis inlet (AC in to Meanwell PSU). Ordered 2026-06-15, ETA late June.
- Custom power cable: TRUE1 male → Edison female pigtail (ships with installation). Connects foot outlet to TRITAP.
- TRITAPs: ship with installation. Used by crew to daisy-chain all PA speakers from the Edison pigtail.
- Touch sensor: TTP223 capacitive touch sensor module (ordered 2026-06-15). The TTP223 pad input connects to a conductive electrode on or within the fiberglass surface; module outputs digital HIGH/LOW to sequencer ESP32 GPIO. One TTP223 per touch zone (snout, horns). Electrode material TBD pending Ruts creative approval — Lucille exploring options: ornate copper lattice, fine copper mesh, or translucent conductive materials. TTP223 is compatible with any of these. Ruts' concern: visible copper through fiberglass. Preferred solutions keep the electrode visually minimal or embedded between fiberglass layers.

**Communication protocol — locked:**
- Sequencer → Audio ESP32: UART (TX→RX wire, no WiFi on audio node)
- Sequencer → WLED ESP32: HTTP GET over WiFi (Sequencer acts as AP, WLED connects as client). WLED's realtime UDP protocol is for raw color streaming, not preset triggering — HTTP GET `/win&PL=N` is the correct way to fire a saved preset.
- No external router or hotspot required

**Physical — locked:**
- Bison ships as 12 components: 4 legs, head, horns, body in 4 pieces, steel base
- Horns and snout: fiberglass/resin (touch zones)
- Body: ¾ pine ply
- Base: square tube metal frame with ground anchor points

**Client ownership:**
- Work for hire. Superfly owns all deliverables after acceptance.
- Luca retains exclusive right of first refusal for 2027+ maintenance (separate agreement).

**Scope:**
- This spec covers 2026 deployment only.

---

## 6) Assumptions

- **A-1:** Audio files from JB (Silent G Studios) will be delivered to Luca in final form by July 10, 2026 (allowing time for loading, testing, and dry-run before July 15-19 ready date). If files are not received by July 5, Luca escalates directly to JB. If not received by July 10, Luca ships with placeholder audio and documents a post-ship remote SD card update procedure for META crew. **Audio specs sent to JB on 2026-06-15** (stereo WAV, 44.1kHz or 48kHz, 16-bit minimum, exact duration required for firmware timer).
- **A-2:** Festival power hookup is a clean 20A / 120V AC circuit. No generator noise or dirty power issues.
- **A-3:** The BRAIN enclosure size has been approved by Lucille and will be hard-mounted in the Bison head. Lucille will pre-plan LED channel mounting paths and cable pass-throughs into the body panels before Luca integrates the tech module. Head assembly sequence (open vs closed relative to sensor wire routing) to be confirmed with Lucille during build coordination. **Lucille MUST also preserve a hollow wire routing path through each horn interior before fiberglass is sealed — horn LED wires must be able to pass from the horn tip back through the horn base into the head where the BRAIN is mounted. This is a pre-fabrication coordination requirement; it cannot be added after the horns are sealed.**
- **A-4:** No external router. The sequencer ESP32 operates as a WiFi access point. The WLED ESP32 connects to it as a client. The audio ESP32 has no WiFi — triggered by UART. No reliance on festival WiFi or external networking hardware.
- **A-5:** The Superfly crew assembling in SF will have basic tools (screwdrivers, cable connectors). No soldering or programming required.
- **A-6:** Rain plan: system is powered off at any rainfall. Tarp is available onsite. Lights are waterproof; speakers and electronics enclosure fan are not rated for direct rain.
- **A-7:** JB and Ruts (META crew) are the onsite team at Outside Lands.
- **A-8:** Touch electrode design requires creative approval from Ruts (META creative director) before Lucille fabricates and integrates it into the fiberglass horns and snout. Ruts' concern: visible copper through translucent fiberglass. Lucille is preparing a sketch and exploring material alternatives — options include ornate copper lattice, fine copper mesh (less visually obtrusive), and translucent conductive materials. Sketch in progress as of 2026-06-15. Firmware development proceeds in parallel — electrode material choice has no impact on firmware. JB is the designated first-line technical contact and follows the installation guide troubleshooting tree for hardware issues. If issues exceed the guide's coverage, Luca is the remote escalation contact (via WhatsApp or equivalent). JB's primary domain is audio content; hardware electronics troubleshooting is covered by the guide with Luca on call.

---

## 7) Deliverables

### Electronics & Hardware
- Tech module (weathersealed enclosure containing sequencer ESP32, WLED ESP32, audio ESP32, power distribution, cooling fan — no router required)
- Touch sensor assembly (snout + horn zones with wiring to sequencer)
- LED strips + aluminum channels, pre-mounted and wired
- Audio output chain (audio ESP32 → RCA → PA + sub)
- All labeled cables with matching connectors
- Custom TRUE1-to-Edison pigtail power cable (foot outlet → TRITAP)
- TRITAPs (quantity: sufficient to connect all PA speakers from one Edison feed)

### Firmware & Software
- Sequencer firmware (touch detection, retrigger lock, UDP broadcast)
- WLED configuration (sequence preset, UDP trigger mapping)
- Audio firmware (SD card playback on UDP trigger)
- Audio files loaded on SD card (from JB)

### Documentation
- **Installation Guide** (for non-technical crew): assembly sequence with photos, cable connection guide, power-on/off procedure, troubleshooting decision tree, rain plan, post-event storage/boxing guide
- **Wiring diagram** (labeled, matching physical cable labels)
- **Spare parts list** (with sourcing links)
- **Pre-ship dry-run video** (full sequence test, all zones, documented before July 21 ship date)
- **Crating specification and packing guide**: padding requirements per component, orientation markings, fragile labels, moisture protection for tech module during Los Cabos to SF transit

---

## 8) Acceptance Criteria (SAT)

- **SAT-1 Touch response:** Touch snout → sequence starts within 250ms. Verify: stopwatch from touch to first LED/audio output. Pass: ≤250ms on 5 consecutive tests.
- **SAT-2 Horn response:** Touch left horn → same sequence. Touch right horn → same sequence. Verify: manual test both horns independently. Pass: both trigger correctly.
- **SAT-3 Retrigger lock:** Touch during active sequence → no restart, no interruption. Verify: touch repeatedly during playback. Pass: sequence plays to completion unaffected.
- **SAT-4 Idle state:** No touch for 60 seconds → LEDs off, no audio. Verify: observe system after sequence completes. Pass: returns to full off within 5 seconds of sequence end.
- **SAT-5 Audio quality:** Play full sequence at intended volume. Verify: listen at 3 meters. Pass: audio clearly audible and undistorted; no clipping or dropouts.
- **SAT-6 Lighting sequence:** Touch triggers full WLED sequence. Verify: observe LEDs through full sequence. Pass: sequence runs to completion, matches intended design, no flickering or dead zones.
- **SAT-7 Power:** System runs for 4 hours continuously in dry-run. Verify: run from power-on, trigger interaction every 10 minutes. Pass: no restarts, no overheating, fan operational. Note: 4-hour test is the accepted proxy for the 11-hour festival operating day — thermal readings at hour 4 MUST show a stable plateau (not still rising); a rising trend at hour 4 indicates a failure mode that will manifest at hours 6-8 in the field.
- **SAT-8 Weather seal:** Inspect enclosure seals and fan vent assembly. Verify: visual inspection + 250ml water pour at 45 degrees over fan vent. Pass: no gaps in enclosure, no water reaches fan blade or enclosure interior after water pour test.
- **SAT-9 Assembly guide:** JB or Ruts (META crew) assemble the tech module and connect all cables using only the installation guide during SF warehouse assembly. Pass: full assembly completed without verbal guidance from Luca. This is the live acceptance test — Luca is available remotely if the guide is unclear.
- **SAT-10 Pre-ship video:** Dry-run video shows all SAT-1 through SAT-8 checks passing. Video shared with client before ship date.

---

## 9) High-Level Architecture

```
[Touch Zones]
  Snout (TTP223 → copper lattice)  ──┐
  Horns (TTP223 → copper lattice)  ──┘
                                      │ digital HIGH
                           [Sequencer ESP32]
                             - Reads TTP223 GPIO
                             - Retrigger lock timer
                             - WiFi Access Point (AP mode)
                             - UDP broadcaster (WLED only)
                             - UART TX (audio trigger)
                                      │
                     ┌────────────────┴─────────────────┐
                     │ UART (TX→RX wire)                 │ UDP over WiFi
                     ↓                                   ↓
          [Audio ESP32]                        [WLED ESP32]
          - No WiFi                            - Connects to Sequencer AP
          - UART trigger → SD playback         - Receives UDP trigger
          - PCM5102A I2S DAC                   - Runs SK6812 sequence
          - RCA output → PA + Sub              - Body channels + horn tips

Power: 20A AC → TRUE1 inlet at foot → BRAIN TRUE1 inlet → 5V 350W Meanwell
       TRUE1 outlet at foot → custom TRUE1→Edison pigtail → TRITAP → PA speakers
WiFi: Sequencer ESP32 is AP. WLED connects as client. Audio ESP32 has no WiFi.
Cooling: Fan in enclosure with downward-facing louver/baffle duct
```

**Data flow on touch event:**
1. TTP223 detects touch on copper lattice → digital HIGH to Sequencer GPIO
2. Sequencer sets retrigger lock ON
3. Sequencer sends UART byte to Audio ESP32 AND UDP packet to WLED ESP32 simultaneously
4. Audio ESP32 plays WAV file from SD card via PCM5102A I2S DAC → RCA → PA
5. WLED runs preset lighting sequence (body channels + horn tip pixels)
6. At sequence end: Sequencer clears retrigger lock via AUDIO_DURATION_MS timer, returns to idle

---

## 10) Risks

| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|------------|
| Touch sensor false triggers in crowded festival environment (RF/EMI noise) | Medium | High | Physical prototyping before July 1; tune sensitivity; add debounce logic in firmware |
| Audio files not delivered by JB before July 10 | Medium | High | Set hard deadline July 10 with JB; have placeholder audio ready by July 5 for firmware testing |
| WiFi interference at festival (crowded RF environment) | Medium | Medium | Use 5GHz band if ESP32 supports it; alternatively switch to ESP-NOW if UDP proves unreliable in testing |
| Shipping damage to tech module | Low | High | Document final state on video before ship; include spare fuses, connectors, one spare ESP32 in shipment |
| Rain event during festival | Medium | Medium | Rain plan documented in installation guide; tarp onsite; system powered off at first rain |
| Crew assembly errors in SF | Medium | Medium | Assembly guide with photos; SAT-9 crew dry-run test before ship; labeled cables |
| Touch zones inaccessible to visitors (structure placement) | Low | High | Coordinate with Lucille on snout/horn height and accessibility during fabrication |
| Overheating in enclosure | Low | Medium | Cooling fan; 4-hour dry-run stress test (SAT-7); thermal check during prototyping |

---

## 11) Open Decisions

| Decision | Options | Recommendation | Status |
|----------|---------|----------------|--------|
| Festival dates | Aug 7-9 vs Aug 8-10 | **Resolved: Aug 7-9** (Friday Aug 7 – Sunday Aug 9, per Q&A doc). Ship date July 21 unaffected. | Closed |
| Audio system final spec | 5.1 surround (original plan) vs 2x PA monitors + sub | **Resolved: 2x PA monitors + sub.** Sub pending budget confirmation. 5.1 is retired. | Closed |
| Touch electrode material | Ornate copper lattice vs fine copper mesh vs translucent conductive film | Lucille exploring all three. Ruts concern: visible copper through fiberglass. Translucent or embedded mesh preferred aesthetically. TTP223 is compatible with any option. Lucille sketch in progress. | Pending Ruts approval |
| WiFi band | 2.4GHz vs 5GHz for ESP32 mesh | Prototype on 2.4GHz; assess interference in dry-run; switch to 5GHz or ESP-NOW if needed | Open |
| Spare parts onsite | None vs minimal kit (fuses, connectors, 1 spare ESP32) | Recommend minimal spare kit — low cost, high value at a remote festival | Pending client decision |
| Horn sensor count | 1 shared input for both horns vs individual inputs | 1 shared input (per Luca) — simpler wiring, acceptable for this use case | Decided |
| Lighting sequence design | TBD during prototyping | Warm/amber/white colors in idle-to-bright burst pattern; finalize in WLED during build phase | In design |
| Router/hotspot placement | Inside tech module vs external | Inside module (weathersealed, no dependency on external network) | Decided |
| LED density | 60 LED/m vs 144 LED/m (SK6812 RGBWW 5V) | 144 LED/m preferred: higher pixel count allows each LED to run at lower brightness for the amber/warm white blend, producing smoother gradients and less heat per LED. Power draw is not a constraint — system will never run all RGBWW channels at full output. 60 LED/m is acceptable if 144 LED/m is unavailable from Parallel stock. | Open — confirm stock availability |
| LED segment count | 3 x 1m segments vs 6 x 1m segments | Start with 3; add 3 more if coverage or brightness is insufficient after physical test | Open — validate in prototyping |
