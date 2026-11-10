# Technical architecture — UE 5.8 First Person

## Current baseline

The locally created First Person template is the authoritative starting point. The GitHub repository initially contains configuration and documentation, not the local Unreal project. **Do not replace template-generated classes or rename modules until the local files have been inspected.**

## Recommended system boundaries

- **Property data:** identified property type, current values, constraints, and validation.
- **Property carrier/component:** attached to eligible world actors; exposes read/apply operations.
- **Extractor interaction:** detects targets, extracts payloads, applies payloads, and gives user feedback.
- **Visual feedback:** current payload, valid/invalid target state, before/after values.
- **Test scenario:** deterministic objects and reset.

Use **C++** for shared property rules and validated transfers, and **Blueprints** for level composition, visual responses, and rapid gameplay tuning.

### Planned names (not implemented)

- `ERealityPropertyType`: Mass, Friction, Heat, Conductivity, Elasticity, Buoyancy.
- `URealityPropertyComponent`: owns and validates eligible property state.
- `UPropertyExtractorComponent`: player-side targeting and payload logic.
- `FPropertyPayload`: type and quantity held by extractor.

These names are provisional. They are not files/classes in the repository.

## Mass / friction technical caveats

- Changing mass in Chaos physics requires care with mass overrides, simulation state, and wake-up behavior.
- Friction is generally associated with physical materials and contact interactions; a game-facing friction property may require material instances or a controlled abstraction.
- Define thresholds, conservation rules, rollback behavior, and feedback before polishing effects.
- Do not assume server-authoritative replication exists in this prototype.

## Testing strategy

1. Build/open the untouched First Person baseline.
2. Verify move/look controls and default map.
3. Introduce one isolated property carrier and check its behavior.
4. Add extractor state and targeting.
5. Add source/target transfer with bounds checks.
6. Assemble a reproducible puzzle and document outcomes.

We will request a screenshot of the Unreal Content Browser and the local project directory before authoring module-specific Unreal C++ files.
