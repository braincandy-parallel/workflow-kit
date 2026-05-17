# Implement Skill — Modes

Curated collections of features. PL frontmatter `mode:` field picks one. Default is implicit if absent.

| Mode | Use Case | Status |
|---|---|---|
| [default](default.md) | Any sprint. Matches current /implement behavior. | default |
| [turbo](turbo.md) | Page surface sprints with locked design tokens. | experimental |
| [swarm](swarm.md) | Large multi-surface sprints with parallel spec work. | experimental |

## Cherry-picking features

A PL can override its mode's feature list:

```yaml
mode: turbo
features:
  add: [multi-surface-parallelism]
  remove: [parallel-harness-author]
  config:
    persistent-dev-server-worker:
      port: 3001
```

If neither `mode:` nor `features:` is present, `mode: default` is loaded.
