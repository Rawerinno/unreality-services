# UNREALITY SERVICES

A surreal first-person co-op puzzle game about municipal technicians repairing failures in the laws of reality.

**Engine:** Unreal Engine 5.8  
**Target:** Windows PC / Steam  
**Gameplay:** First person; solo prototype, eventual 1–4-player cooperation  
**Status:** Pre-production / prototype 0.0.1  
**Core mechanic:** Property Routing

## Premise

You work for a municipal reality-maintenance service. When everyday physics malfunctions, your crew extracts physical properties from one object and routes them to another. The results can solve puzzles, restore infrastructure, or make things worse.

## Physical properties

- **Mass** — affects weight and physical response.
- **Friction** — controls surface grip and sliding.
- **Heat** — enables thermal interactions.
- **Conductivity** — routes electricity through materials.
- **Elasticity** — determines springiness and deformation response.
- **Buoyancy** — controls how objects float or sink.

The initial playable slice uses **Mass** and **Friction** only. The other four are deferred until the interaction loop is fun and stable.

## Version 0.0.1: acceptance criteria

1. Launch into a single test room using the existing UE 5.8 First Person template.
2. Move, aim, and interact with physics-enabled props.
3. Aim at an eligible source object and extract a permitted amount of Mass or Friction.
4. Store one selected property payload in the Property Extractor.
5. Aim at a valid target and transfer that payload.
6. Observe a clear, reversible physics change.
7. See understandable feedback when an interaction is invalid.
8. Restart the test scenario without requiring an editor restart.

**Not in 0.0.1:** networking, Steam integration, finished art, elaborate UI, save games, and the remaining four properties.

## Getting started

1. Install UE 5.8, Visual Studio with C++ tooling, Git, and Git LFS.
2. Run `git lfs install` once on your machine.
3. Clone this repository **before** copying your Unreal project into it.
4. Copy the local Unreal project's `.uproject`, `Config/`, `Content/`, and (if present) `Source/` and `Plugins/` into the repository root, preserving existing folders.
5. Run `git status`, inspect the staged file list, and commit the template project.
6. Open the `.uproject` using Unreal Engine 5.8.

**Important:** This repository currently contains project planning and Git configuration only. It does not yet contain a playable Unreal project or verified compiled game code. Do not create empty placeholder assets with `.uasset` or `.umap` extensions.

For more detail, see [Development setup](Docs/DEVELOPMENT.md), [Game design](Docs/GAME_DESIGN.md), and [Architecture](Docs/ARCHITECTURE.md).

## Collaboration

- Keep changes small and descriptive.
- Do not commit generated folders (`Saved/`, `Intermediate/`, `Binaries/`, `DerivedDataCache/`).
- Track binary UE assets via Git LFS.
- Prototype gameplay rules in a test room before creating content-heavy maps.
