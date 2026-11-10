# Game design — UNREALITY SERVICES

## Fantasy

Players are underfunded municipal technicians dispatched to fix absurd breakdowns of reality. Mundane work orders become physics puzzles, and every repair risks an unpredictable chain reaction.

## Pillars

1. **Readable physical causes and effects.** Every property transfer visibly changes the world.
2. **Experimentation first.** More than one solution should be possible where feasible.
3. **Cooperative chaos.** Future tasks should create opportunities to coordinate or accidentally interfere.
4. **Low ceremony.** Few controls and fast resets mean players spend time experimenting, not navigating menus.

## Property Routing

A handheld **Property Extractor** has two stages:

1. **Extract:** Select a compatible source and capture an amount of a physical property.
2. **Apply:** Target another compatible object and transfer the held payload.

In prototype 0.0.1, only one payload may be held. The UI must always disclose its type, amount, and whether a target is eligible.

### Properties and intended gameplay

| Property | Intended effect | Phase |
|---|---|---|
| Mass | Change mass of physics props | 0.0.1 |
| Friction | Change how strongly an object grips surfaces | 0.0.1 |
| Heat | Heating/cooling interactions | Later |
| Conductivity | Make or break electrical paths | Later |
| Elasticity | Change rebound/spring response | Later |
| Buoyancy | Change float/sink behavior | Later |

These describe gameplay goals, not claims that each maps directly to a single Unreal physics parameter.

## Prototype rules (provisional; tune by playtesting)

- Only marked props are valid sources or targets.
- Extraction and transfer must honor configured minimum/maximum property values.
- A transfer conserves the extracted payload within the game rule system; creation of unlimited property values is not allowed.
- Property readings use clear *game units*, not necessarily real-world SI values.
- Support undo/reset of a puzzle.
- Invalid interactions do not silently change an object's state.

## First test-room puzzle

A heavy movable block obstructs a path. An eligible lightweight block and a low-friction sliding surface provide alternate uses for extracted Mass or Friction. Players should be able to explain why the resulting motion changes.

## Out of scope for now

No combat, crafting economy, persistence, elaborate narrative systems, or networking in 0.0.1.

## Open design decisions

- Whether the extractor carries a fixed or variable quantity.
- How much state is stored per property at once.
- Whether friction changes the object, its contact material, or designated surfaces.
- Multiplayer authority and ownership of transfers.
