---
date created: 2026-06-15
tags: [review, parallel-solo, outside-lands]
category: Review
---
# ARE - Outside Lands Bison Installation Scope Analysis

## Affected Components

| Component | Owner | Integration Point with Luca | Notes |
|-----------|-------|----------------------------|-------|
| Sequencer ESP32 | Luca | Core controller; reads capacitive sensors, enforces retrigger lock, broadcasts UDP | Firmware is Luca's full build |
| WLED ESP32 | Luca | Receives UDP trigger from sequencer; drives SK6812 strips | WLED firmware + preset configuration |
| Audio ESP32 | Luca | Receives UDP trigger from sequencer; plays SD card file to PA via RCA | SD card loaded with JB audio content pre-ship |
| Capacitive touch sensors (snout + horns) | Luca | Sensors wire to sequencer ESP32 GPIO; physical mounting points on snout/horn are Lucille's structure | Sensor type TBD pending prototyping (ordered 2026-06-15); must be validated before July 1 |
| SK6812 RGBWW LED strips | Luca (Parallel stock) | Strips mount inside aluminum channels; channels attach to Bison body (Lucille's scope) | IP65 rated or sealed required; mount points must be pre-planned by Lucille |
| Aluminum LED channels | Luca (Parallel stock) | Attached to interior Bison body panels (Lucille's scope) | Lucille must confirm routing paths and mounting method before Luca can pre-mount strips |
| Weathersealed tech module (enclosure) | Luca | Module sits inside Bison body cavity; must be removable as a unit (FR-23) | Bison body cavity dimensions from Lucille required before enclosure sizing |
| Power distribution inside module | Luca | Single 20A / 120V AC inlet from festival; Luca owns everything from inlet inward | Labeled, detachable cables (FR-20) |
| Cooling fan | Luca | Fan mounted in enclosure; vent cutouts in Bison body may require coordination with Lucille if body is not pre-cut | Rain baffle required (FR-19) |
| Dedicated WiFi router/hotspot | Luca | Housed inside tech module; no external network dependency | 2.4GHz vs 5GHz band still open; ESP-NOW fallback if UDP proves unreliable |
| PA monitor speakers (2x) | Luca (sourcing) | External to Bison body; crew positions per installation guide | Sub pending budget confirmation |
| Subwoofer | Luca (sourcing, conditional) | External to Bison body; crew cables to audio ESP32 RCA output | Budget gate still open |
| Touch zone physical surfaces (snout, horns) | Lucille | Fiberglass/resin; capacitive sensors must bond to or sit behind these surfaces | Sensor attachment method must be validated against fiberglass during prototyping — fiberglass can shield capacitive signal |
| Bison body structural panels (4 pieces) | Lucille | 3/4 pine ply; interior surface used for LED channel mounting and cable routing | Lucille must route or pre-drill cable pass-throughs |
| Steel base with ground anchor points | Lucille | Base is Lucille's scope; Luca has no electronics in the base | Luca needs to know base height to verify snout/horn accessibility (risk R-7 in spec) |
| 4 legs (crated components) | Lucille | Structural only; no electronics in legs | Leg-to-body connection sequence may affect tech module insertion order |
| Head assembly | Lucille | Touch sensors attach to snout within head; head must mate with body before or after tech module install | Assembly sequence dependency: head attachment before or after sensor wiring must be defined in guide |

## External Dependencies

| Dependency | Owner | Risk | Deadline |
|------------|-------|------|---------|
| Audio content (WAV/MP3 files) | JB (Silent G Studios) | HIGH -- late delivery blocks firmware audio testing, SD card loading, and the dry-run. No audio = no SAT-5, SAT-10 | July 10, 2026 (A-1). Hard deadline. Luca should have placeholder audio ready by July 5 for firmware testing |
| PA system sourcing (2x monitors + sub) | Luca (must source or confirm rental) | MEDIUM -- sub is budget-gated and unconfirmed. If sub is cut, audio SPL at 3m (NFR-6) must be re-validated with monitors only | Must be confirmed before dry-run (target ~July 15-19) |
| Festival power circuit (20A / 120V AC) | Superfly / festival operations | MEDIUM -- spec assumes clean power (A-2). Generator noise or shared circuit could cause ESP32 instability or audio artifacts | Must be confirmed with Superfly before ship. If unclean, add power conditioner to BOM |
| Structure fabrication readiness (Lucille) | Lucille | HIGH -- Luca cannot size enclosure, route LED channels, or validate sensor attachment until body cavity dimensions and mounting plans are confirmed | Luca needs Lucille's interior dimensions and mounting plan before enclosure build starts. No stated deadline in spec -- this is a gap |
| Snout/horn height confirmation (Lucille) | Lucille | MEDIUM -- if snout/horn are not at visitor-accessible height, touch interaction fails entirely (spec risk R-7) | Needs to be locked before Lucille finalizes structure; Luca should request a sketch or dimension sheet |
| Festival WiFi environment data | Superfly | LOW-MEDIUM -- system uses dedicated router, but crowded 2.4GHz RF at a festival can still cause UDP packet loss affecting sequencer-to-nodes comms | Request RF environment guidance from Superfly; test with 5GHz if ESP32 variant supports it |
| Superfly dry-run video sign-off | Superfly | LOW -- client must sign off before July 21 ship date | Video must be shared before July 21; sign-off is gate for shipping |

## Documentation Effort Estimate

| Deliverable | Complexity | Notes |
|-------------|-----------|-------|
| Installation Guide | HIGH | Audience is non-technical festival crew. Must cover: 12-component assembly sequence with photos, cable connection guide with labeled diagrams, power-on/off procedure, troubleshooting decision tree, rain plan, and post-event storage/boxing guide. SAT-9 requires a live dry-run with an unfamiliar crew member -- the guide must be complete before that test. Estimate: 1-2 days of focused writing and photography after build is complete and stable |
| Wiring diagram | MEDIUM | Must label every cable with matching physical labels on actual cables (FR-22). Needs to be created during build (not after) so labels are consistent. One diagram is probably sufficient but must be clear enough for a non-technical reader. Estimate: half day, done iteratively during wiring |
| Spare parts list with sourcing links | LOW | Straightforward BOM subset: fuses, connectors, 1x spare ESP32, possibly spare LED strip segment. Sourcing links to Amazon/Adafruit/AliExpress. Estimate: 2-3 hours |
| Pre-ship dry-run video | MEDIUM | Must demonstrate SAT-1 through SAT-8 passing. Requires a stable, fully integrated system. Needs to be filmed, lightly edited or captioned, and shared with Superfly before July 21. Estimate: 1 day (4-hour continuous run per SAT-7 + filming + basic edit). Gate: JB audio files must be loaded (July 10 dependency) |

## Risk Surface

| Risk Area | What Could Go Wrong |
|-----------|-------------------|
| Capacitive sensor + fiberglass | Fiberglass/resin can attenuate or block capacitive fields depending on thickness. If the sensor is mounted behind the fiberglass shell, sensitivity may be inadequate or require a thicker conductor pad. Must be validated in physical prototyping before July 1 -- if sensor type is wrong, there is not enough time to pivot late |
| Sensor false triggers in festival crowd | Dense crowds generate body capacitance and RF noise. The sequencer must have well-tuned debounce and sensitivity thresholds. If not tuned properly, the system will trigger constantly or not at all. Firmware debounce and threshold tuning is a required prototyping output, not a post-build adjustment |
| UDP reliability in crowded RF environment | Three ESP32 nodes communicating over WiFi UDP in a dense festival environment (2.4GHz congestion from thousands of phones). A missed UDP packet means WLED or audio does not trigger -- visible failure. Mitigation path (ESP-NOW) is noted in spec but not yet tested; needs to be evaluated in prototype phase |
| Audio delivery from JB (single point of failure) | JB is the only source of audio content. If files are delivered late, low quality, or at the wrong level, Luca has no time to iterate with JB after July 10. No escalation path is documented in the spec. A placeholder audio file should be ready by July 5 so firmware testing is not blocked |
| Enclosure sizing locked to Lucille's body cavity | Luca cannot finalize the tech module enclosure dimensions until Lucille confirms the interior body cavity size. If Lucille runs late or changes the design, the enclosure may need to be rebuilt. This dependency has no stated deadline in the spec and is a schedule risk |
| LED channel mounting before body assembly | LED strips and channels must be mounted inside the body panels before the Bison is fully assembled. If Lucille's panel sequence does not allow access after assembly, channels must be pre-mounted before the body is closed. This requires a coordinated assembly sequence that is not yet documented |
| Head/sensor assembly sequence | It is unclear whether the head attaches to the body before or after the touch sensor wires are routed to the tech module. If the head is a closed fiberglass shell, sensor wiring may need to be pre-routed through the head before attachment -- a step that must be captured in the installation guide |
| Shipping damage to tech module | The tech module contains three ESP32 boards, a router, power distribution, and an SD card with audio content. A single hard impact could knock loose connectors or corrupt the SD card. The module must be documented (video) in its final state before packing. Spare ESP32 and fuses in the shipment are critical |
| 12-component crating and reassembly | Twelve crated components must survive Los Cabos to San Francisco transit. If crating is not engineered (padding, orientation markings, moisture protection), fragile fiberglass horns or LED channels could arrive damaged. Crating plan is not addressed in the spec |
| Non-technical crew assembly errors | Assembly errors in SF could range from wrong cable connections to structural misalignment. The installation guide must anticipate every failure mode and the troubleshooting tree must cover the most likely mistakes (wrong connector polarity, LED strip reversed, router not powering on). SAT-9 crew dry-run is the only validation gate -- if that test is skipped, assembly errors at festival are likely |
| JB as onsite technical contact | A-7 names JB as the onsite technical contact during the festival. JB is also the audio content provider. If JB is unavailable (illness, scheduling conflict), there is no named backup for onsite hardware issues. This is a single point of failure at the event |
| Rain event and power-off procedure | The rain plan requires crew to power off the system at first rain and deploy a tarp. If crew are not trained on the power-off procedure or the tarp is not staged nearby, electronics could be exposed to direct water. The installation guide must include the rain plan prominently, not buried in appendices |
| Subwoofer budget gate | If the sub is cut, NFR-6 (clearly audible at 3m in a loud festival environment) must be re-validated with monitors only. A bison roar or low-frequency audio content without a sub may not meet SPL requirements. This decision must be made before the dry-run so audio is tested with the actual hardware configuration |
| Year-over-year durability vs 2026 build pressure | NFR-5 requires commodity/replaceable components for YOY reuse. Build-time shortcuts (hot glue, non-labeled cables, non-standard connectors) that save time now will make 2027 maintenance difficult. This is a quality discipline risk throughout the build phase |
