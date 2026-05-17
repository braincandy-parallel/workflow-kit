---
feature: dev-server-concurrency-contract
status: default-on
modifies: [SKILL.md:quality]
requires: []
conflicts: []
default_params: {}
source_retro: ""
---

# Dev Server Concurrency Contract

## What it does
Encodes a contract for sharing the dev server cache between dev and build processes. Only one actor owns the dev server at a time; `pnpm build` never runs while a dev server is active in the same worktree; verifier rebuilds always kill the dev server, clear the cache, build, then restart; parallel workers running their own dev servers each get an explicit per-worker port; production-grade isolation uses `NEXT_BUILD_DIR=.next-build`. Eliminates the non-deterministic `ENOENT pages-manifest.json` failure mode that occurs when dev and build write the same cache directory concurrently.

## Activation
Per sprint, per worker that touches the dev server or a build. Documented in the bracket or team config at sprint start, and enforced on every worker brief that includes dev-server or build steps.

## Modification: SKILL.md:quality
The following block is injected into SKILL.md under the "Dev Server Concurrency Contract" section:

> The dev server (e.g., `pnpm --filter web dev`) and the production build (`pnpm --filter web build`) both write to the same cache directory (e.g., `apps/web/.next/`). Running them concurrently produces non-deterministic `ENOENT pages-manifest.json` failures.
>
> Rules:
> - Only one actor owns the dev server at a time. Document the owner in the bracket or team config.
> - Never run `pnpm build` while a dev server is active in the same worktree.
> - If a verifier needs to run a fresh build, the dev server is killed first (`pkill -f "next dev"`), the cache cleared (`rm -rf apps/web/.next apps/web/.turbo`), build runs, then dev server is restarted.
> - For parallel workers each running their own dev servers, use a per-worker port (3000, 3010, 3011, …) explicitly assigned in the worker brief.
> - For production-grade isolation, set `NEXT_BUILD_DIR=.next-build` on the build invocation so it uses a separate directory.
>
> Source: real-world parallel-worker sprint retro.

## Escape valve
For sprints with no dev server or no build step in scope, the contract is a no-op and may be omitted from the worker briefs. For non-Next.js stacks, the orchestrator adapts the kill/clear/build/restart commands to the framework's equivalents and notes the adaptation in the bracket. There is no runtime toggle; the contract is structural.

## Expected delta vs not-having-it
Targets the analogous failure mode observed in real-world sprints and #6 (verifier-d connection-refused during a coordinator-triggered restart). Both incidents stemmed from undefined dev-server ownership during multi-actor verification. With the contract default-on, ownership is explicit, the kill/clear/build/restart sequence is mandatory, and the ENOENT and connection-refused classes drop out of the incident log.
