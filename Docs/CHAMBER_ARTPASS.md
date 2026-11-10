# UNREALITY SERVICES — Chamber art pass v1

## Why the first room appeared dark

The original generated map was an empty indoor level with only a directional light and four weak point lights. It had no authored sky/environment setup and essentially no bounced light. In an enclosed space, a directional light cannot illuminate hidden surfaces.

## This update

The new **non-destructive** \`Scripts/upgrade_test_chamber.py\` updates the **existing** L_TestChamber_01. It preserves the original floor, walls, mass blocks and player spawn, and adds:

- Slate-blue walls, matte industrial floor, steel supports and bright accent materials
- Cyan intake lane and amber delivery lane, luminous guide markings
- Suspended glowing ceiling panels, structural trim and clean geometric wall features
- Seven shadow-free high-reach point lights plus brighter existing lamps and a directional fill light
- Colored A and B physics cubes, if those actors are present

The art pass is a **stylized blockout**, not a full AAA asset pass. It uses engine cubes and simple material graphs for speed, maintainability and low GPU requirements. Lighting is intentionally strong; the next visual pass will tune exposure, add decals/signage, improve silhouettes and replace fixtures with authored assets.

## Apply on your computer

**Important: save any current level changes and close Unreal Editor before changing Git branches.** The map you generated exists only on your local machine until you commit it.

From PowerShell:
\`\`\`powershell
cd C:\Dev\unreality-services
git status --short
git add Content/UnrealityServices/Maps
git commit -m "feat: save locally generated test chamber"
git fetch origin
git switch feature/chamber-artpass-lighting
git pull --ff-only
\`\`\`

If Git reports **nothing to commit**, that's fine; the map may already be saved and tracked.

In Unreal Engine 5.8:
1. Open your project from \`C:\Dev\unreality-services\UNREALITY_SERVICES.uproject\`.
2. Open **Content > UnrealityServices > Maps > L_TestChamber_01** (the existing room).
3. Verify that the **Python Editor Script Plugin** is enabled.
4. Open **Output Log** and run:
   \`\`\`
   py "C:/Dev/unreality-services/Scripts/upgrade_test_chamber.py"
   \`\`\`
5. Wait for the message \`Art pass complete\`. The script saves the updated level.
6. Choose **Lit** viewport mode. Play and test the two cubes.

If Python raises an exception, copy the **first traceback** from Output Log. Don't delete the level.

Once it works, send the updated .umap and material assets back to GitHub:
\`\`\`powershell
cd C:\Dev\unreality-services
git add Content/UnrealityServices
git commit -m "feat: save test chamber art pass assets"
git push origin feature/chamber-artpass-lighting
\`\`\`

Your .umap and .uasset files should be stored via Git LFS. The script creates native material assets inside Unreal; those aren't available from the GitHub text connector until you push them.

## Graphics performance

The added fill lamps are non-shadow-casting to reduce overhead. Materials are simple and do not use expensive textures. You can change quality in the prototype with **F1**, and quality keys **1–4** while the panel is open.

For a modest PC, start with Medium or Low and 70–80% render scale. We will measure actual frame time in a later test using \`stat fps\` and \`stat unit\`; a performance claim would be premature now.

## Known limitations

- No built editor validation or packaged game performance measurements yet.
- Light intensity/tonemapping may vary by project exposure and scalability level.
- Actor placement and wall materials are intentionally modular to facilitate later revision.
- A stylized authored weapon, full skeletal animation and final VFX are separate tasks.
