# Test Chamber 01 — local map generation

This feature adds a repeatable UE 5.8 level creation script in \`Scripts/create_test_chamber.py\`.

## Why a script?

Unreal Engine .umap files are native binary assets created by the Unreal Editor. GitHub's text-file connector cannot generate a valid .umap. The script creates and saves one in the installed editor, then it can be committed with Git LFS.

## One-time setup

1. Close the Unreal editor. Use PowerShell from \`C:\Dev\unreality-services\`:
   \`\`\`powershell
   git fetch origin
   git switch feature/dedicated-test-chamber
   git pull --ff-only
   \`\`\`
2. Open \`UNREALITY_SERVICES.uproject\` with Unreal Engine 5.8. Ensure the C++ code compiles first.
3. Enable **Edit > Plugins > Python Editor Script Plugin** and restart the editor if requested. This plugin is editor-only.
4. Choose **Tools > Execute Python Script** and select \`C:\Dev\unreality-services\Scripts\create_test_chamber.py\`.
5. Open \`Content/UnrealityServices/Maps/L_TestChamber_01\` if not opened automatically, then press Play.

The generator makes:
- 26 m x 19 m industrial test bay with walls, rails, elevated lights, two marked pads and distance strips
- Spawn location facing the property transfer zone
- BODY_A (20 kg), BODY_B (5 kg), both backed by C++ physics-enabled actors
- No imported art, external plugins, Niagara dependencies or heavy ray tracing

The existing default map configuration is **not changed until the generated .umap exists**. Once created:
- Go to **Edit > Project Settings > Maps & Modes**.
- Set **Editor Startup Map** and **Game Default Map** to \`L_TestChamber_01\`.
- Save and commit the generated level and updated \`Config/DefaultEngine.ini\`:

\`\`\`powershell
git add Content/UnrealityServices/Maps Config/DefaultEngine.ini
git commit -m "feat: add generated test chamber map"
git push origin feature/dedicated-test-chamber
\`\`\`

To ensure C++ prototype blocks are not spawned twice, the next C++ change detects placed blocks in the level before creating fallback test cubes. The original First Person template map remains intact.

## Troubleshooting

- \`RealityMassBlock class unavailable\`: rebuild the C++ Editor target, restart the editor and rerun the script.
- \`Python editor plugin missing\`: enable Python Editor Script Plugin, restart.
- Script errors: open **Window > Developer Tools > Output Log**, copy the first Python exception and provide it for fixing.
- File already exists: the script refuses to overwrite it; reopen the level instead.
- Unexpectedly empty room: check the correct map path and the generated level in Content Drawer.

**Status:** the script has been committed to GitHub, but execution and map visual output must be tested in Unreal Editor.
