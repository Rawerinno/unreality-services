# First playable interaction — UE 5.8 editor setup

**Status:** implementation proposed in a branch; compile and Play-in-Editor have not yet been verified on Windows.

## Compile

1. Close Unreal Editor.
2. Pull/check out the branch after review, or merge the PR then pull main.
3. Open `UNREALITY_SERVICES.uproject` and build the C++ editor target in Visual Studio as needed.
4. Do not delete/recreate the First Person map.

## Add interactable objects

1. Open `Content/FirstPerson/Lvl_FirstPerson`.
2. Add two **Static Mesh Actor** cubes, each with a mesh collision that blocks Visibility traces.
3. Turn on **Simulate Physics** in each cube's Static Mesh component.
4. Add the C++ **Reality Property Component** to each actor using the Details panel.
5. Set StartingMassKg=20 on cube A and StartingMassKg=5 on cube B. Set MinimumMassKg=1 and MaximumMassKg=50 on both.
6. Press Play and confirm the cubes are physically simulated.

## Connect the extractor

1. Open `Content/FirstPerson/Blueprints/BP_FirstPersonCharacter`.
2. Add the C++ **Property Extractor Component**.
3. For a quick no-asset prototype, add keyboard input events in the character Blueprint using the existing Enhanced Input mapping context (create two Input Actions for Extract and Apply, and add bindings to IMC_Default).
4. Wire the Extract action to `Extract Mass from View`; wire Apply to `Apply Mass to View`.
5. Print the returned bool and Held Mass Kg for diagnosis.
6. Aim at cube A, extract 5 kg; aim at cube B and apply 5 kg. Cube A should reach 15 kg, cube B 10 kg.

## Known limitations

- The current carrier assumes a single simulating primitive component. It is not a multi-body skeletal system.
- A payload remains in the extractor until successfully applied; no discard/return mechanism yet.
- A different actor is required as the destination; sending mass back to the source is not supported yet.
- There is no network replication or authoritative server path.
- UI and input assets still require editor authoring, and the feature is unverified until the local build/test.
- Property state is runtime-only. Reopening or restarting Play resets from configuration.
- The game-default map and player class remain untouched.
