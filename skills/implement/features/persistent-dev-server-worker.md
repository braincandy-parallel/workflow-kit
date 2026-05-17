---
feature: persistent-dev-server-worker
status: experimental
modifies:
  - SKILL.md:dispatch
requires: []
conflicts: []
default_params:
  port: 3000
  health_check_path: /
  restart_protocol: clean-cache-then-restart
source_retro: ""
---

# Persistent Dev Server Worker

## What it does

Spawns a dedicated `dev-server-owner` worker at sprint start whose sole responsibility is owning the dev server lifecycle (start, health, restart, cache clears, port management). All implementation workers and verifiers query this worker via SendMessage rather than starting, stopping, or restarting their own dev server processes.

The owner serializes restart requests, applies the configured restart protocol (default: clean cache then restart), and reports the live port + health status back to requesters.

## Activation

BEFORE the first batch dispatch in the sprint. Coordinator spawns `dev-server-owner` as part of pre-bootstrap, alongside any other persistent infrastructure workers. It stays alive for the duration of the sprint.

## Modification: SKILL.md:dispatch

Adds a pre-bootstrap step ahead of the first batch:

1. Spawn `dev-server-owner` worker with `default_params` (port, health_check_path, restart_protocol).
2. Worker brings the dev server up and reports ready.
3. All subsequent worker dispatch templates instruct workers to query `dev-server-owner` via SendMessage instead of running `npm run dev`, `next dev`, or equivalent.
4. Verifier templates likewise route restart requests through the owner.

## Escape valve

If `dev-server-owner` becomes unresponsive or misbehaves, the coordinator kills the worker and falls back to the baseline behavior: workers manage their own dev server in their own panes, and the coordinator absorbs restart coordination. No file-level rollback required.

## Expected delta vs not-having-it

Eliminates the connection-refused and `pages-manifest.json` race class caused by multiple workers starting, killing, and restarting the dev server against the same port. Phase 2 lost roughly 20 minutes total to these races (collision retries, cache-corruption restarts, port confusion across panes). With the persistent owner, the race surface collapses to a single owned process.
