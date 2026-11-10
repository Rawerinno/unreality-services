# Developer setup

## Requirements

- Windows machine
- Unreal Engine **5.8**
- Visual Studio with C++ game development tools and Windows SDK
- Git and Git LFS

## Clone and import existing local project

Do not blindly run `git init` inside a second repository or overwrite the local template. Use this safe flow:

```powershell
cd C:\Dev
git clone https://github.com/Rawerinno/unreality-services.git
cd .\unreality-services
git lfs install
```

Then close Unreal Editor and Visual Studio. Using File Explorer, **copy** (do not move) these items from your existing UE 5.8 First Person project into `C:\Dev\unreality-services\`:

- `<YourProject>.uproject`
- `Config\`
- `Content\`
- `Source\` (if it exists)
- `Plugins\` (only if your project actually uses plugins)

Preserve the generated project name and module name until they have been inspected. If any destination files already exist, inspect differences before replacing them.

Check what will be committed:

```powershell
git status --short
git lfs ls-files
git add .
git status --short
git commit -m "chore: import UE 5.8 first person starter project"
git push origin main
```

If `git lfs ls-files` is empty **before** `git add`, that is normal. After staging, verify that binary `.uasset` / `.umap` files are LFS-tracked (`git lfs ls-files` after committing is definitive).

## Before import

A valid Unreal project requires its original `.uproject` descriptor. Never invent or rename project modules to match the GitHub repo's kebab-case name.

## Troubleshooting

If the project fails to open or compile, share:

- Exact `.uproject` filename and whether the project is Blueprint-only or C++.
- Contents of `Source/`, if any.
- Unreal/Visual Studio error text.

Do not delete the original local Unreal project until the imported copy has been opened successfully.
