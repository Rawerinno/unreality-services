# UNREALITY SERVICES — Playable slice 0.0.1

**Status: code committed to a feature branch. Build and runtime validation are pending.**

## Objective

Prove the central gameplay loop using the UE 5.8 First Person template. A technician takes 5 kg from Body A, routes it to Body B, and uses an identical push impulse to compare the blocks.

## Included in code

- First-person extractor housing, barrel and grip made from placeholder engine cube meshes
- Native hand-tool recoil animation and short colored debug energy-line feedback
- Runtime test bodies: A starts at 20 kg, B starts at 5 kg
- Transfer: A becomes 15 kg and B becomes 10 kg
- Fixed-force impulse (F) to compare acceleration under different masses
- Screen overlay: technician name, XP, crosshair, target mass, reservoir status and mission prompts
- F1 graphics overlay: Low/Medium/High/Epic, render scale, V-Sync
- Graphics settings use Unreal GameUserSettings persistence

## Controls

| Key | Action |
| --- | --- |
| WASD + mouse | Default First Person movement/look |
| E | Extract 5 kg from the target |
| R | Apply the held 5 kg to a different target |
| F | Apply fixed impulse to the physical target |
| F1 | Toggle graphics overlay |
| 1 / 2 / 3 / 4 | Low / Medium / High / Epic while overlay open |
| [ / ] | Decrease / increase render scale 10 percentage points |
| V | Toggle V-Sync while overlay open |

## First local build

1. Close the Unreal Editor.
2. From `C:\Dev\unreality-services`, run:
   ```powershell
   git fetch origin
   git switch feature/first-playable-slice
   git pull --ff-only
   ```
3. Double-click `UNREALITY_SERVICES.uproject` and approve module rebuild if prompted. If build fails, open the generated solution in Visual Studio and build Development Editor / Win64. Share the first compiler error, not just the final summary.
4. Open the default First Person map and press Play.
5. Look for two test cubes approximately 4.6 m in front of the initial spawn and offset 1.6 m left/right. They spawn on the existing level surface; a separate authored test-room asset is not included.
6. Look at body A; HUD should show 20 kg. Press E. It should show 15 kg and a 5 kg payload.
7. Look at body B; press R. It should show 10 kg and payload 0.
8. Aim at each block in turn; press F to compare motion. With equal impulses, the lighter block should gain more speed, although collisions and friction also affect the result.
9. Press F1 and try Medium and 70% render scale on modest hardware. Close the overlay and continue gameplay.

## Known limitations

- The temporary test cubes spawn relative to the local character, not as saved level actors. In busy or enclosed spawn spaces they may need editor placement later.
- The block geometry and tool shape are placeholders; the beam is a debug draw, not production VFX.
- Visual HUD is drawn using the native Canvas renderer; future UMG widgets will permit responsive layout, animations and controller input.
- XP is a local prototype counter, capped at 100, with no saved progression.
- Render scale changes may behave differently in PIE than in a packaged build. Verify on a standalone/packaged build before calling graphics options complete.
- Multiplayer is intentionally unsupported.
- Neither UE compilation nor runtime tests have been executed remotely.

## Quality gates before merging to main

- [ ] C++ compiles cleanly with UE 5.8
- [ ] Test cubes spawn in walkable locations
- [ ] Extractions and transfers preserve total mass
- [ ] Invalid targets leave payload unchanged
- [ ] Recoil and feedback visible
- [ ] Graphics options persist after restart
- [ ] Play can restart without failure

## Next studio pass

Author the dedicated environment map and real tool silhouette, swap debug energy line for Niagara, add sound, aim targeting animation, improved UI layout, save/load profile, and physics test instrumentation.
